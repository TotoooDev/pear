#version 330 core
out vec4 frag_color;

#define NUM_MAX_LIGHT 128

struct Material {
    sampler2D texture_albedo;
    sampler2D texture_specular;
    sampler2D texture_normal;

    vec4 color_diffuse;
    vec4 color_specular;

    float roughness;
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

void main() {
    DirectionalLight light = u_directional_lights[0];
    vec3 ambient = light.ambient * u_material.color_diffuse.rgb;

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * u_material.color_diffuse.rgb;

    vec3 view_dir = normalize(u_cam_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.roughness);
    vec3 specular = light.specular * spec * u_material.color_diffuse.rgb;

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
}
