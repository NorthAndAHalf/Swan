#pragma once

#include "RenderPipeline.h"
#include <functional>
#include <memory>
#include "Renderer/FrameBuffer.h"

class BasicPipeline : public RenderPipeline
{
public:
	BasicPipeline();

	virtual void render() override;
private:
	std::vector<std::function<void(std::shared_ptr<FrameBuffer>, std::shared_ptr<FrameBuffer>)>> passes;
};
