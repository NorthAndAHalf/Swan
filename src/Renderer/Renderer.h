#pragma once

#include <vector>
#include "RenderPipelines/RenderPipeline.h"
#include <memory>

class Renderer
{
public:
	Renderer();

	std::shared_ptr<RenderPipeline> get_pipeline();
	void set_pipeline(std::shared_ptr<RenderPipeline> _pipeline);

	void update();

private:
	std::shared_ptr<RenderPipeline> pipeline;
};
