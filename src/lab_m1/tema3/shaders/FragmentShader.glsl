#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec2 fragTexture;
in vec3 fragColor;

// Output
layout(location = 0) out vec4 out_color;

uniform sampler2D texture_1;
uniform int has_texture;
uniform int moving;
uniform float Time;

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 point_light_color[2];
uniform vec3 point_light_pos[2];
uniform vec3 point_light_dir[2];
uniform int type[2];
uniform float cut_off;

uniform vec3 bec_far_pos;
uniform vec3 bec_far_dir;
uniform vec3 bec_far_color;

vec3 point_light_contribution(vec3 light_pos, vec3 light_color, vec3 light_dir, int type) {
    float ambient_light = 0.0;
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
    if (type == 1) {
        float cutoff = radians(cut_off);
        light_dir = normalize(light_dir);
        float spot_light = dot(-L, light_dir);
        float spot_light_limit = cos(cutoff);
        if(spot_light > spot_light_limit) {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);
            light = ambient_light + light_att_factor * (diffuse_light + specular_light); 
        } else {
            light = ambient_light;
        }
    }
    if (type == 2) {
        L = normalize(-light_dir);
        H = normalize(L + V);
        R = reflect(-L, world_normal_copy);
        float diffuse_light = material_kd * max(dot(world_normal_copy, L), 0);
        if (diffuse_light > 0)
        {
            specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
        }
        attenuation = 1.0 / (1 + 0.2 * d);
        light = (diffuse_light + specular_light) * attenuation;
    }
    color = light_color * light; 
    return color;
}

void main()
{
    vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (has_texture == 1) {
        float x = fragTexture.x;
        float y = fragTexture.y;
        if (moving == 1){
            float wave = sin(world_position.x * 10.0 + Time) * 0.2 + 10;
            vec2 modulatedCoords = fragTexture + vec2(wave / 100.0, 0.0);
            color = texture2D(texture_1, vec2(modulatedCoords.x + Time / 50.f, modulatedCoords.y));
        }
        else
            color = texture2D(texture_1, fragTexture);
    } else {
        color = vec4(fragColor, 1.0f);
    }
    
    color.rgb *= 0.1;
    for(int i = 0; i < 2; i++) {
        vec3 color_copy;
        if (i == 1)
            color_copy = fragColor;
        else
            color_copy = point_light_color[i];

        vec3 light_contribution = point_light_contribution(point_light_pos[i], color_copy, point_light_dir[i], type[i]);
        color.rgb += light_contribution;
    }
    color.rgb += point_light_contribution(bec_far_pos, bec_far_color, bec_far_dir, 0);
    out_color = color;

}
