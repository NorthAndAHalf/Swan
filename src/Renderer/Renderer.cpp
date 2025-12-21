#include "Renderer.h"
#include "Core.h"

Renderer::Renderer()
{
}

std::shared_ptr<RenderPipeline> Renderer::get_pipeline()
{
    return pipeline;
}

void Renderer::set_pipeline(std::shared_ptr<RenderPipeline> _pipeline)
{
    pipeline = _pipeline;
}

void Renderer::update()
{
    SF_ASSERT(pipeline, "Render pipeline is null");

    pipeline->render();
}
