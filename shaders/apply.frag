#version 460 core

void defaultFrag(in vec4 inColor, out vec4 outColor);

in vec4 outColor;
out vec4 fragColor;


void main()
{
	vec4 FinalColor;
	defaultFrag(outColor, FinalColor);
	fragColor = FinalColor;
}
