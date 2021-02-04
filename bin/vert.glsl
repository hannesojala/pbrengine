#version 460 core

in vec3 a_position;
in vec3 a_normal;
in vec2 a_uvCoords;

out vec2 uvCoords;
out vec3 normal;
  
uniform mat4 u_mvp;

void main() {
    uvCoords = a_uvCoords;
    normal = a_normal;
    gl_Position = u_mvp * vec4(a_position, 1.0);
}