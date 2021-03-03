#version 430 core

in vec2 TextureOut;

layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D texture2;

void main()
{
	color = texture(texture2, TextureOut) + vec4(0.3, 0.3, 0.3, 1.0);
}