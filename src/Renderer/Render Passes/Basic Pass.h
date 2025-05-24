#pragma once
#include "Renderer/FrameBuffer.h"
#include <memory>

void BasicPass(std::shared_ptr<FrameBuffer> inputFrameBuffer, std::shared_ptr<FrameBuffer> outputFrameBuffer);
