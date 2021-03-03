#version 430 core

in vec2 TextureOut;

layout(location = 0) out vec4 color;

layout(location = 0) uniform sampler2D texture1;

void main()
{
	color = texture(texture1, TextureOut) + vec4(0.3, 0.3, 0.3, 1.0);
}