#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 color;

out vec3 aColor;

uniform vec2 uResolution;

void main() {
  vec2 norm = aPos / uResolution;
  vec2 ndc = norm * 2.0 - 1.0;

  ndc.y = -ndc.y;

  gl_Position = vec4(ndc, 0.0, 1.0);
  gl_PointSize = 1.0;
  aColor = color;
}