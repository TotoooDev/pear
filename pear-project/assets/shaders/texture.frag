#version 330 core
out vec4 frag_color;

// FIXME: anything more than that throws error C6020 and complains that there are too many uniforms in the shader (i think)
// i should be moving this stuff in a UBO soon because 48 lights of each type is tiny
#define NUM_MAX_LIGHT 48

struct Material {
    sampler2D texture_albedo;
    sampler2D texture_specular;
    sampler2D texture_normal;

    vec4 color_diffuse;
    vec4 color_specular;

    float roughness;

    // FIXME: ugly fucking code
    bool has_texture_specular;
};

struct DirectionalLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 pos;

    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 pos;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cut_off;
    float outer_cut_off;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 texture_coords;
in vec3 normal;
in vec3 frag_pos;

uniform vec3 u_cam_pos;
uniform Material u_material;
uniform DirectionalLight u_directional_lights[NUM_MAX_LIGHT];
uniform PointLight u_point_lights[NUM_MAX_LIGHT];
uniform SpotLight u_spot_lights[NUM_MAX_LIGHT];
uniform int u_num_directional_lights;
uniform int u_num_point_lights;
uniform int u_num_spot_lights;

vec3 calculate_directional_light(DirectionalLight light, vec3 norm, vec3 view_dir) {
    vec3 ambient = light.ambient * texture(u_material.texture_albedo, texture_coords).rgb;

    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(u_material.texture_albedo, texture_coords).rgb;

    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.roughness);
    // ewwwww
    vec3 specular;
    if (u_material.has_texture_specular)
        specular = light.specular * spec * texture(u_material.texture_specular, texture_coords).rgb;
    else
        specular = light.specular * spec * u_material.color_specular.rgb;

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 calculate_point_light(PointLight light, vec3 norm, vec3 view_dir) {
    vec3 light_dir = normalize(light.pos - frag_pos);
    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * texture(u_material.texture_albedo, texture_coords).rgb * attenuation;

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(u_material.texture_albedo, texture_coords).rgb * attenuation;

    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.roughness);
    vec3 specular;
    if (u_material.has_texture_specular)
        specular = light.specular * spec * texture(u_material.texture_specular, texture_coords).rgb * attenuation;
    else
        specular = light.specular * spec * u_material.color_specular.rgb * attenuation;

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 calculate_spot_light(SpotLight light, vec3 norm, vec3 view_dir) {
    vec3 light_dir = normalize(-light.direction);
    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    float theta = dot(light_dir, light_dir);
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * texture(u_material.texture_albedo, texture_coords).rgb * attenuation * intensity;

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(u_material.texture_albedo, texture_coords).rgb * attenuation * intensity;

    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.roughness);
    vec3 specular;
    if (u_material.has_texture_specular)
        specular = light.specular * spec * texture(u_material.texture_specular, texture_coords).rgb * attenuation * intensity;
    else
        specular = light.specular * spec * u_material.color_specular.rgb * attenuation * intensity;

    vec3 result = ambient + diffuse + specular;
    return result;
}

void main() {
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(u_cam_pos - frag_pos);
    vec3 final_color = vec3(0.0);

    for (int i = 0; i < u_num_directional_lights; i++)
        final_color += calculate_directional_light(u_directional_lights[i], norm, view_dir);
    for (int i = 0; i < u_num_point_lights; i++)
        final_color += calculate_point_light(u_point_lights[i], norm, view_dir);
    for (int i = 0; i < u_num_spot_lights; i++)
        final_color += calculate_spot_light(u_spot_lights[i], norm, view_dir);
    
    frag_color = vec4(final_color, 1.0);
}
