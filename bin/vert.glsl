#version 460 core

in vec3 a_position;
in vec2 a_uvCoords;

out vec2 o_uvCoords;
  
uniform mat4 u_mvp;

void main() {
    o_uvCoords = a_uvCoords;
    gl_Position = u_mvp * vec4(a_position, 1.0);
}