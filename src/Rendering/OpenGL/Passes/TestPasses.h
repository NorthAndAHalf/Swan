#pragma once
#include "Rendering/OpenGL/RendererCore.h"
#include "Rendering/OpenGL/Framebuffer.h"
#include "Rendering/OpenGL/ShaderProgram.h"

void TestPass(ScreenQuad& quad, Framebuffer& writeBuffer, ShaderProgram& shader);
void TestInversePass(ScreenQuad& quad, Framebuffer& readBuffer, Framebuffer& writeBuffer, ShaderProgram& shader);