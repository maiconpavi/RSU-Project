#version 460 core



void defaultVert(in mat4 pv, in vec3 iPosition, in vec3 iColor, out vec4 pos, out vec4 color) {
    pos = pv * vec4(iPosition, 1.0f);
	color = vec4(iColor, 1.0f);
}
