#include "BasicPipeline.h"
#include "Renderer/Render Passes/Basic Pass.h"

BasicPipeline::BasicPipeline()
{
	passes.push_back(BasicPass);
}

void BasicPipeline::render()
{
	std::shared_ptr<FrameBuffer> tempInput = std::make_shared<FrameBuffer>();
	std::shared_ptr<FrameBuffer> tempOutput = std::make_shared<FrameBuffer>();

	for (std::function<void(std::shared_ptr<FrameBuffer>, std::shared_ptr<FrameBuffer>)> pass : passes)
	{
		pass(tempInput, tempOutput);
	}
}
