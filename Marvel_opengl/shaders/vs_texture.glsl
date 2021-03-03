#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 Texture;

out vec2 TextureOut;

layout(location = 20) uniform mat4 model;
layout(location = 40) uniform mat4 modelView;
layout(location = 60) uniform mat4 modelViewProj;

void main()
{
	gl_Position = modelViewProj * vec4(Position, 1.0);
	TextureOut = Texture;
}