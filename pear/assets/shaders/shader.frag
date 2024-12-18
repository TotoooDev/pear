#version 330 core

#define NUM_MAX_LIGHTS 128u
#define LIGHT_TYPE_DIRECTIONAL 0u
#define LIGHT_TYPE_POINT 1u
#define LIGHT_TYPE_SPOT 2u

out vec4 frag_color;

struct light_t {
    uint type;

    vec3 pos;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outer_cutoff;
};

struct material_t {
    sampler2D diffuse;
    sampler2D specular;
    vec3 color;
    float shininess;
};

in VS_OUT {
    vec3 frag_pos;
    vec2 texture_coords;
    vec3 normal;
} fs_in;

layout (std140) uniform ubo_lights {
    uint u_num_lights;
    vec3 u_view_pos;
    light_t u_lights[NUM_MAX_LIGHTS];
};

uniform material_t u_material;

vec3 calculate_directional_light(light_t light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), u_material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, fs_in.texture_coords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, fs_in.texture_coords));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, fs_in.texture_coords));
    return (ambient + diffuse + specular);
}

vec3 calculate_point_light(light_t light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.pos - frag_pos);
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), u_material.shininess);
    // attenuation
    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, fs_in.texture_coords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, fs_in.texture_coords));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, fs_in.texture_coords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calculate_spot_light(light_t light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.pos - frag_pos);
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    vec3 halfway_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), u_material.shininess);
    // attenuation
    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(light_dir, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_material.diffuse, fs_in.texture_coords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_material.diffuse, fs_in.texture_coords));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, fs_in.texture_coords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main() {
    vec3 norm = normalize(fs_in.normal);
    vec3 view_dir = normalize(u_view_pos - fs_in.frag_pos);

    vec3 result = vec3(0.0);
    for (uint i = 0u; i < u_num_lights && i < NUM_MAX_LIGHTS; i++) {
        light_t light = u_lights[i];

        switch (light.type) {
        case LIGHT_TYPE_DIRECTIONAL:
            result += calculate_directional_light(u_lights[i], norm, view_dir);
            break;

        case LIGHT_TYPE_POINT:
            result += calculate_point_light(light, norm, fs_in.frag_pos, view_dir);
            break;

        case LIGHT_TYPE_SPOT:
            result += calculate_spot_light(light, norm, fs_in.frag_pos, view_dir);
            break;
        }
    }

    frag_color = vec4(result, 1.0);
}
