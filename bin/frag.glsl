#version 460 core

in vec2 uv_coords;
in vec3 normal;
in vec3 frag_pos;
in vec3 tangent;
in vec3 bitangent;

out vec4 FragColor;

uniform sampler2D u_diffuse_map;
uniform sampler2D u_mtl_rgh_map;
uniform sampler2D u_normal_map;
uniform vec4 u_view_pos;

const float PI = 3.1415;

float NDF_DistGGX(float ndh, float roughness) {
    float roughnessSqrd = roughness*roughness;
    float ndhSqrd = ndh*ndh;
    float denom = (ndhSqrd * (roughnessSqrd - 1.0) + 1.0);
    denom = PI * denom * denom;

    return roughnessSqrd / denom;
}

float Geom_SmithSchlickGGX(float ndv, float ndl, float k) {
    float ggx1 = ndv / (ndv * (1.0 - k) + k);
    float ggx2 = ndl / (ndv * (1.0 - k) + k);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosAng, vec3 f0) {
    return mix(f0, vec3(1.0), pow(1.01 - cosAng, 5.0));
}

struct Light {
    vec3 pos;
    vec3 clr;
};

Light LIGHTS[] = Light[](
    Light(vec3(512), vec3(1000000))
);
  
void main() {
    float gamma = 2.2;
    mat3 tbn = mat3(tangent, bitangent, normal);
    vec3 norm_map = normalize(texture(u_normal_map, uv_coords).rgb * 2.0 - 1.0);
    vec3 norm = normalize(tbn * norm_map);
    vec3 albedo = pow(texture(u_diffuse_map, uv_coords).xyz, vec3(gamma));
    vec3 mtl_rgh = texture(u_mtl_rgh_map, uv_coords).xyz;
    float metalness = mtl_rgh.b;
    float roughness = clamp(pow(mtl_rgh.g, 2.0), 0.00025, 1.0);
    
    vec3 view_dir = normalize(u_view_pos.xyz - frag_pos);
    vec3 base_refl = vec3(0.04);

    vec3 clr = vec3(0.0);
    
    for (int i = 0; i < LIGHTS.length(); i++) {
        vec3 light_pos = LIGHTS[i].pos;
        vec3 light_clr = LIGHTS[i].clr;
        vec3 light_dir = normalize(light_pos - frag_pos);
        float dist = length(light_pos - frag_pos);
        float ndl = max(dot(norm, light_dir), 0.0);
        float ndv = max(dot(norm, view_dir), 0.0);
        vec3 halfway = normalize(light_dir + view_dir);
        float ndh = max(dot(norm, halfway), 0.0);

        vec3 f0 = mix(base_refl, albedo, metalness);

        float D = NDF_DistGGX(ndh, roughness);
        float k = (roughness+1.0) * (roughness+1.0) / 8.0; // supposedly different for image based lighting
        float G = Geom_SmithSchlickGGX(ndv, ndl, k);
        vec3 F = fresnelSchlick(ndv, f0);

        vec3 tc_specular = f0 * D * G * F * light_clr;
        vec3 tc_diffuse = albedo * light_clr * (1.0-metalness);

        clr += ndl * (tc_diffuse + tc_specular) / ((dist+1.0) * (dist+1.0));
    }

    clr += (0.001 * albedo);

    // Some tone mapping?
    // clr = clr / (clr + vec3(1.0));

    // final Gamma correction
    clr = pow(clr, vec3(1.0/gamma));

    FragColor = vec4(clr, 1.0);
}