#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 point_light_color[1];
uniform vec3 point_light_pos[1];
uniform vec3 point_light_dir[1];
uniform float cut_off;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

vec3 point_light_contribution(vec3 light_pos, vec3 light_color, vec3 light_dir) {
    float ambient_light = 0.25;
    float specular_light = 0;
    vec3 world_normal_copy = world_normal;
    world_normal_copy = normalize(world_normal_copy);
    vec3 color;
    vec3 L = normalize(light_pos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = reflect(-L, world_normal_copy);
    float diffuse_light = material_kd * max(dot(world_normal_copy, L), 0);
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }
    float d = distance(light_pos, world_position);
    float attenuation = 1.0 / (1 + 0.1 * d + 0.05 * d * d);
    float light = ambient_light + attenuation * (diffuse_light + specular_light);
    // if (type != 0) {
    //     float cutoff = radians(cut_off);
    //     light_dir = normalize(light_dir);
    //     float spot_light = dot(-L, light_dir);
    //     float spot_light_limit = cos(cutoff);
    //     if(spot_light > spot_light_limit) {
    //         float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
    //         float light_att_factor = pow(linear_att, 2);
    //         light = ambient_light + light_att_factor * (diffuse_light + specular_light); 
    //     } else {
    //         light = ambient_light;
    //     }
    // }
    color = light_color * light; 
    return color;
}

void main()
{
    vec3 color = vec3(0, 0, 0);
    for(int i = 0; i < 1; i++)
    {
        color += point_light_contribution(point_light_pos[i], point_light_color[i], point_light_dir[i]);
    }
    out_color = vec4(color, 1);
}
