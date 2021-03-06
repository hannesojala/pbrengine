#version 460 core

in vec3 a_position;
in vec3 a_normal;
in vec3 a_tangent;
in vec2 a_uv_coords;

out vec2 uv_coords;
out vec3 normal;
out vec3 frag_pos;
out vec3 tangent;
out vec3 bitangent;
  
uniform mat4 u_mvp;
uniform mat4 u_model;

void main() {
    uv_coords = a_uv_coords;
    normal = (u_model * vec4(a_normal, 1.0)).xyz;
    tangent = a_tangent;
    bitangent = -normalize(cross(normal, tangent));
    frag_pos = (u_model * vec4(a_position, 1.0)).xyz;
    gl_Position = u_mvp * vec4(a_position, 1.0);
}