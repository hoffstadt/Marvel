#version 430 core

layout(location = 0) out vec4 color;

layout(location = 4) uniform vec4 cbuf;

void main()
{
	color = cbuf;
	//color = vec4(1.0, 1.0, 1.0, 1.0);
}