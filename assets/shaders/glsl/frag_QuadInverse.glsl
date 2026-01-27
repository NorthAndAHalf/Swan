#version 460

in vec2 uv;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
	vec4 frameColor = texture(screenTexture, uv);
	FragColor = vec4(1.0) - frameColor;
	FragColor.w = frameColor.w; // Don't invert alpha
}