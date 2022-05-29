#version 460 core

flat in vec3 startPos;
in vec3 vertPos;
in vec4 Color;

uniform vec2 resolution;
uniform uint pattern;
uniform float factor;

void main() {
  vec2 dir = (vertPos.xy - startPos.xy) * resolution / 2.0;
  float dist = length(dir);

  uint bit = uint(round(dist / factor)) & 15U;
  if ((pattern & (1U << bit)) == 0U)
    discard;
  gl_FragColor = Color;
}
