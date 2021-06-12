#version 330 core

in vec3 vp;
in vec3 vn;
out vec3 vn_out;

uniform mat4 translation;
uniform mat4 view;
uniform mat4 perspective;

void main(){
  vn_out = vn;
  gl_Position = perspective * view * translation * vec4(vp, 1.0);
}
