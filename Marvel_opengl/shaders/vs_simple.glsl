#version 430 core

layout(location = 0) in vec4 Position;

layout(location = 20) uniform mat4 model;
layout(location = 40) uniform mat4 modelView;
layout(location = 60) uniform mat4 modelViewProj;

void main()
{
	gl_Position = modelViewProj * Position;
}