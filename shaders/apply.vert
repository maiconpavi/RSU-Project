#version 460 core


void defaultVert(in mat4 pv, in vec3 iPosition, in vec3 iColor, out vec4 pos, out vec4 color);

// Ponto3D
layout (location = 0) in vec3 p_inPos;
// RGB
layout (location = 1) in vec3 p_inColor;

uniform mat4 pv;

out vec4 outColor;



void main()
{
	vec4 outPos;

	defaultVert(pv, p_inPos, p_inColor, outPos, outColor);

	gl_Position = outPos;
}
