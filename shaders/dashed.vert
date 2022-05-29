#version 460 core

layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColor;

flat out vec3 startPos;
out vec3 vertPos;

uniform mat4 pv;


out vec4 Color;

void main() {
  gl_Position = pv * vec4(iPosition, 1.0f);
  vertPos = gl_Position.xyz;
  startPos = vertPos;

  Color = vec4(iColor, 1.0f);
}
