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
    vec4 frag_pos_light_space;
} fs_in;

layout (std140) uniform ubo_lights {
    uint u_num_lights;
    vec3 u_view_pos;
    light_t u_lights[NUM_MAX_LIGHTS];
};

uniform material_t u_material;
uniform sampler2D u_shadow_map;

float calculate_shadow_directional(vec4 frag_pos_light_space, vec3 normal, vec3 light_dir) {
    // perform perspective divide
    vec3 projection_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    // transform to [0,1] range
    projection_coords = projection_coords * 0.5 + 0.5;
    if (projection_coords.z > 1.0) {
        return 0.0;
    }
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closest_depth = texture(u_shadow_map, projection_coords.xy).r; 
    // get depth of current fragment from light's perspective
    float current_depth = projection_coords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(u_shadow_map, 0);
    for (int x = -1; x < 1; x++) {
        for (int y = -1; y < 1; y++) {
            float pcf_depth = texture(u_shadow_map, projection_coords.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

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
    // shadow stuff
    float shadow = calculate_shadow_directional(fs_in.frag_pos_light_space, normal, light_dir);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture(u_material.diffuse, fs_in.texture_coords).rgb; 
    
    return lighting;
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
    if (texture(u_material.diffuse, fs_in.texture_coords).a < 0.1) {
        discard;
    }

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
