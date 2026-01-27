#include "TestPasses.h"

void TestPass(ScreenQuad& quad, Framebuffer& writeBuffer, ShaderProgram& shader)
{
	// Bind VAO before calling this function
	glViewport(0, 0, writeBuffer.GetWidth(), writeBuffer.GetHeight());
	glEnable(GL_DEPTH_TEST);
	quad.DrawToFramebuffer(shader, writeBuffer);
}

void TestInversePass(ScreenQuad& quad, Framebuffer& readBuffer, Framebuffer& writeBuffer, ShaderProgram& shader)
{
	glViewport(0, 0, writeBuffer.GetWidth(), writeBuffer.GetHeight());
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, readBuffer.GetColorTexture());
	quad.DrawToFramebuffer(shader, writeBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}