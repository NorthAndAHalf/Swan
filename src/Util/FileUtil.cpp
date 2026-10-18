#include "FileUtil.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdint>

#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../Engine/Engine.h"

#define CGLTF_IMPLEMENTATION
#include "../../vendor/cgltf/cgltf.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb/stb_image.h"


namespace FileUtil
{
    std::string ReadFileToString(const char* path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file)
        {
            spdlog::warn("File not found: {0}", path);
            return "";
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string buffer;
        buffer.reserve(size);

        buffer.assign((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        return buffer;
    }

    namespace
    {
        constexpr uint32_t NO_TEXTURE = UINT32_MAX;

        struct SubmeshTextures
        {
            uint32_t albedo = NO_TEXTURE;
            uint32_t normal = NO_TEXTURE;
            uint32_t orm = NO_TEXTURE;
            uint32_t emissive = NO_TEXTURE;
        };

        unsigned char* DecodeImage(const cgltf_options& options, const cgltf_image& img,
            const std::string& baseDir, int& w, int& h)
        {
            int channels = 0;

            if (img.buffer_view) {                                   // embedded (GLB)
                const uint8_t* bytes = (const uint8_t*)img.buffer_view->buffer->data + img.buffer_view->offset;
                return stbi_load_from_memory(bytes, (int)img.buffer_view->size, &w, &h, &channels, 4);
            }

            if (img.uri && strncmp(img.uri, "data:", 5) == 0) {      // base64 data URI
                const char* b64 = strchr(img.uri, ',');
                if (!b64) return nullptr;
                b64++;

                size_t len = strlen(b64);
                size_t size = len / 4 * 3;
                if (len >= 1 && b64[len - 1] == '=') size--;
                if (len >= 2 && b64[len - 2] == '=') size--;

                void* bytes = nullptr;
                if (cgltf_load_buffer_base64(&options, size, b64, &bytes) != cgltf_result_success)
                    return nullptr;

                unsigned char* px = stbi_load_from_memory((const unsigned char*)bytes, (int)size, &w, &h, &channels, 4);
                free(bytes);   // cgltf uses malloc by default
                return px;
            }

            if (img.uri) {                                           // external file
                std::string uri(img.uri);
                cgltf_decode_uri(uri.data());                        // undo %20 etc, in place
                uri.resize(strlen(uri.c_str()));
                return stbi_load((baseDir + uri).c_str(), &w, &h, &channels, 4);
            }

            return nullptr;
        }
    }

    Mesh LoadMesh(std::string path)
    {
        cgltf_options options = {};
        cgltf_data* data = nullptr;

        if (cgltf_parse_file(&options, path.c_str(), &data) != cgltf_result_success) {
            spdlog::error("Failed to parse {}", path);
            return Mesh();
        }
        if (cgltf_load_buffers(&options, data, path.c_str()) != cgltf_result_success) {
            spdlog::error("Failed to load buffers for {}", path);
            cgltf_free(data);
            return Mesh();
        }
        if (cgltf_validate(data) != cgltf_result_success) {
            spdlog::error("Corrupt or inconsistent mesh file: {}", path);
            cgltf_free(data);
            return Mesh();
        }

        const std::string baseDir = path.substr(0, path.find_last_of("/\\") + 1);

        // ---------------- Textures ----------------
        std::vector<uint32_t> poolSrgb(data->images_count, NO_TEXTURE);
        std::vector<uint32_t> poolLinear(data->images_count, NO_TEXTURE);

        auto ImageIndex = [&](const cgltf_texture_view& v) -> int {
            return (v.texture && v.texture->image) ? (int)(v.texture->image - data->images) : -1;
            };

        auto UploadTexture = [&](const cgltf_texture_view& view, bool srgb) -> uint32_t {
            const int idx = ImageIndex(view);
            if (idx < 0) return NO_TEXTURE;

            uint32_t& slot = srgb ? poolSrgb[idx] : poolLinear[idx];
            if (slot != NO_TEXTURE) return slot;

            int w = 0, h = 0;
            unsigned char* px = DecodeImage(options, data->images[idx], baseDir, w, h);
            if (!px) {
                spdlog::warn("Failed to decode image {} in {}", idx, path);
                return NO_TEXTURE;   // caller substitutes a DEFAULT TEXTURE
            }
            slot = Engine::Renderer().AddTextureToPool((uint32_t)w, (uint32_t)h, 4, px, srgb);
            stbi_image_free(px);
            return slot;
            };

        // glTF keeps occlusion (R) and metallic-roughness (G = roughness, B = metallic) separate.
        // Your Submesh has one ORM texture, so pack them: R = AO, G = roughness, B = metallic.
        std::map<std::pair<int, int>, uint32_t> ormCache;

        auto BuildOrm = [&](const cgltf_texture_view& mr, const cgltf_texture_view& ao) -> uint32_t {
            const int mrI = ImageIndex(mr), aoI = ImageIndex(ao);
            if (mrI < 0 && aoI < 0) return NO_TEXTURE;
            if (mrI >= 0 && mrI == aoI) return UploadTexture(mr, false);   // already a packed ORM image

            const auto key = std::make_pair(mrI, aoI);
            if (auto it = ormCache.find(key); it != ormCache.end()) return it->second;

            int mw = 0, mh = 0, aw = 0, ah = 0;
            unsigned char* mrPx = mrI >= 0 ? DecodeImage(options, data->images[mrI], baseDir, mw, mh) : nullptr;
            unsigned char* aoPx = aoI >= 0 ? DecodeImage(options, data->images[aoI], baseDir, aw, ah) : nullptr;

            if (mrPx && aoPx && (mw != aw || mh != ah)) {
                spdlog::warn("AO and metallic-roughness sizes differ in {}, ignoring AO", path);
                stbi_image_free(aoPx);
                aoPx = nullptr;
            }
            if (!mrPx && !aoPx) return NO_TEXTURE;   // caller substitutes a DEFAULT TEXTURE

            const int w = mrPx ? mw : aw;
            const int h = mrPx ? mh : ah;
            std::vector<uint8_t> orm((size_t)w * h * 4);
            for (size_t i = 0; i < (size_t)w * h; i++) {
                orm[i * 4 + 0] = aoPx ? aoPx[i * 4 + 0] : 255;   // AO (missing = unoccluded)
                orm[i * 4 + 1] = mrPx ? mrPx[i * 4 + 1] : 255;   // roughness
                orm[i * 4 + 2] = mrPx ? mrPx[i * 4 + 2] : 0;     // metallic
                orm[i * 4 + 3] = 255;
            }

            const uint32_t result = Engine::Renderer().AddTextureToPool((uint32_t)w, (uint32_t)h, 4, orm.data(), false);
            if (mrPx) stbi_image_free(mrPx);
            if (aoPx) stbi_image_free(aoPx);
            ormCache[key] = result;
            return result;
            };

        std::vector<SubmeshTextures> matTextures(data->materials_count);
        std::vector<bool> matDone(data->materials_count, false);

        auto GetMaterialTextures = [&](const cgltf_material* m) -> SubmeshTextures {
            SubmeshTextures t;
            size_t idx = 0;

            if (m) {
                idx = m - data->materials;
                if (matDone[idx]) return matTextures[idx];

                t.albedo = UploadTexture(m->pbr_metallic_roughness.base_color_texture, true);
                t.normal = UploadTexture(m->normal_texture, false);
                t.orm = BuildOrm(m->pbr_metallic_roughness.metallic_roughness_texture, m->occlusion_texture);
                t.emissive = UploadTexture(m->emissive_texture, true);
            }

            // Missing slots (no material, no texture in that slot, or a failed decode)
            if (t.albedo == NO_TEXTURE) t.albedo = Engine::Renderer().GetDefaultAlbedo();
            if (t.normal == NO_TEXTURE) t.normal = Engine::Renderer().GetDefaultNormal();
            if (t.orm == NO_TEXTURE) t.orm = Engine::Renderer().GetDefaultOrm();
            if (t.emissive == NO_TEXTURE) t.emissive = Engine::Renderer().GetDefaultEmissive();

            if (m) {
                matTextures[idx] = t;
                matDone[idx] = true;
            }
            return t;
            };

        // ---------------- Submeshes ----------------
        std::vector<Submesh> submeshes;

        auto BuildSubmesh = [&](const cgltf_primitive& prim, const glm::mat4& world) {
            if (prim.type != cgltf_primitive_type_triangles) {
                spdlog::error("Cannot import primitive type {}, only triangles are supported", (int)prim.type);
                return;
            }

            size_t vertexCount = 0;
            for (size_t a = 0; a < prim.attributes_count; a++) {
                if (prim.attributes[a].type == cgltf_attribute_type_position)
                    vertexCount = prim.attributes[a].data->count;
            }
            if (vertexCount == 0) return;

            MeshData meshData;
            meshData.vertices.resize(vertexCount, Vertex{});

            for (size_t a = 0; a < prim.attributes_count; a++) {
                const cgltf_attribute& attr = prim.attributes[a];
                const cgltf_accessor* acc = attr.data;
                if (acc->count != vertexCount) continue;

                const size_t comps = cgltf_num_components(acc->type);
                std::vector<float> f(acc->count * comps);
                cgltf_accessor_unpack_floats(acc, f.data(), f.size());

                for (size_t v = 0; v < vertexCount; v++) {
                    Vertex& vert = meshData.vertices[v];
                    const float* p = &f[v * comps];

                    switch (attr.type) {
                    case cgltf_attribute_type_position:
                        if (comps == 3) vert.position = glm::vec3(p[0], p[1], p[2]);
                        break;
                    case cgltf_attribute_type_normal:
                        if (comps == 3) vert.normal = glm::vec3(p[0], p[1], p[2]);
                        break;
                    case cgltf_attribute_type_tangent:
                        if (comps == 4) vert.tangent = glm::vec4(p[0], p[1], p[2], p[3]);
                        break;
                    case cgltf_attribute_type_texcoord:
                        if (attr.index == 0 && comps == 2) vert.uv = glm::vec2(p[0], p[1]);
                        break;
                    default:
                        break;
                    }
                }
            }

            const glm::mat3 linear(world);
            const glm::mat3 normalMat = glm::transpose(glm::inverse(linear));
            for (Vertex& v : meshData.vertices) {
                v.position = glm::vec3(world * glm::vec4(v.position, 1.0f));
                if (glm::dot(v.normal, v.normal) > 0.0f)
                    v.normal = glm::normalize(normalMat * v.normal);
                const glm::vec3 t = linear * glm::vec3(v.tangent);
                if (glm::dot(t, t) > 0.0f)
                    v.tangent = glm::vec4(glm::normalize(t), v.tangent.w);
            }

            if (prim.indices) {
                for (size_t k = 0; k < prim.indices->count; k++)
                    meshData.indices.push_back((uint32_t)cgltf_accessor_read_index(prim.indices, k));
            }
            else {
                for (size_t k = 0; k < vertexCount; k++)
                    meshData.indices.push_back((uint32_t)k);
            }

            if (glm::determinant(linear) < 0.0f) {
                for (size_t k = 0; k + 2 < meshData.indices.size(); k += 3)
                    std::swap(meshData.indices[k + 1], meshData.indices[k + 2]);
            }

            const SubmeshTextures t = GetMaterialTextures(prim.material);

            Submesh sub = {};
            sub.m_bufferLocation = Engine::Renderer().GetMeshPool()->StoreMeshData(meshData);
            sub.m_albedoTextureIndex = t.albedo;
            sub.m_normalTextureIndex = t.normal;
            sub.m_ormTextureIndex = t.orm;
            sub.m_emissiveTextureIndex = t.emissive;
            submeshes.push_back(sub);
            };

        for (size_t n = 0; n < data->nodes_count; n++) {
            const cgltf_node& node = data->nodes[n];
            if (!node.mesh) continue;

            float m[16];
            cgltf_node_transform_world(&node, m);
            const glm::mat4 world = glm::make_mat4(m);

            for (size_t j = 0; j < node.mesh->primitives_count; j++)
                BuildSubmesh(node.mesh->primitives[j], world);
        }

        cgltf_free(data);

        if (submeshes.empty()) {
            spdlog::error("No importable triangle meshes in {}", path);
            return Mesh();
        }

        Submesh* start = nullptr;
        for (size_t i = 0; i < submeshes.size(); i++) {
            Submesh* stored = static_cast<Submesh*>(Engine::Renderer().StoreSubmesh(submeshes[i]));
            if (!stored) {
                spdlog::error("Submesh buffer full while importing {}", path);
                return Mesh();
            }
            if (i == 0) start = stored;
            else if (stored != start + i) {
                spdlog::error("Submesh storage is not contiguous for {}", path);
                return Mesh();
            }
        }

        return Mesh(start, (uint32_t)submeshes.size());
    }
}