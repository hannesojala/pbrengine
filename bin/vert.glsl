#version 460 core

in vec3 a_position;
in vec3 a_normal;
in vec2 a_uvCoords;

out vec2 uvCoords;
out vec3 normal;
out vec3 frag_pos;
  
uniform mat4 u_mvp;
uniform mat4 u_model;

void main() {
    uvCoords = a_uvCoords;
    normal = a_normal;
    frag_pos = (u_model * vec4(a_position, 1.0)).xyz;
    gl_Position = u_mvp * vec4(a_position, 1.0);
}