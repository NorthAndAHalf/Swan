#version 460

in vec2 uv;
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, uv.x, uv.y, 1.0f);
}