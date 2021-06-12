#version 330 core

in vec3 vn_out;
out vec3 color;

void main() {
  float light = max(0.5, dot(vn_out, vec3(1.0, 1.0, 0.0)));
  color = vec3(1.0, 0.0, 0.0) * light;
}
