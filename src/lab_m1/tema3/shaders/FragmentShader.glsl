#version 330

// Input
in vec3 world_position;
in vec3 world_normal;
in vec2 fragTexture;
in vec3 fragColor;

// Output
layout(location = 0) out vec4 out_color;

uniform sampler2D texture_1;
uniform int has_texture;
uniform int moving;
uniform int isMoon;
uniform float Time;

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 point_light_color[6];
uniform vec3 point_light_pos[6];
uniform vec3 point_light_dir[6];
uniform float cutoff;

uniform vec3 bec_far_pos;
uniform vec3 bec_far_dir;
uniform vec3 bec_far_color;

uniform vec3 spot_far_pos1;
uniform vec3 spot_far_dir1;
uniform vec3 spot_far_color1;

uniform vec3 spot_far_pos2;
uniform vec3 spot_far_dir2;
uniform vec3 spot_far_color2;

uniform vec3 moon_light_pos;
uniform vec3 moon_light_dir;
uniform vec3 moon_light_color;

uniform vec3 isBoat;

vec3 point_light_contribution(vec3 light_pos, vec3 light_color, vec3 light_dir, int type) {
    float ambient_light = 0.0;
    float specular_light = 0;
    vec3 color;
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_pos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = normalize(reflect(-L, N));

    float diffuse_light = material_kd * max(dot(normalize(N), L), 0.f);

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
    }
    float d = distance(light_pos, world_position);
    float attenuation = 1.f / max(1.f, d * d * 0.1f);
    float light = ambient_light + attenuation * (diffuse_light + specular_light);
    if (type == 1) {
        float cut_off_rad = radians(cutoff);
		float spot_light = dot(-L, light_dir);
		float spot_light_limit = cos(cut_off_rad);
		if (spot_light > spot_light_limit)
		{	 
			float linear_att = (spot_light - spot_light_limit) / (1.f - spot_light_limit) ;
			light = ambient_light + linear_att * (diffuse_light + specular_light);
		} else
		{
			light = ambient_light;
		}
    }
    if (type == 2) {
        L = normalize(-light_dir);
        H = normalize(L + V);
        R = normalize(reflect(-L, N));
        float diffuse_light = material_kd * max(dot(N, L), 0);
        if (diffuse_light > 0)
        {
            specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
        }
        light = (diffuse_light + specular_light);
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
            float wave = (sin(world_position.x * 4.5) + 1) * 0.5;
            vec2 modulatedCoords = fragTexture + vec2(wave / 100.f + Time / 2.f, 0.0);
            color = texture2D(texture_1, vec2(modulatedCoords.x, modulatedCoords.y));
        }
        else
            color = texture2D(texture_1, fragTexture);
    } else {
        color = vec4(bec_far_color, 1.0f);
    }
    
    if(has_texture != 0 || moving != 0)
        color.rgb *= 0.1;
    for(int i = 0; i < 6; i++) {
        vec3 light_contribution = point_light_contribution(point_light_pos[i], point_light_color[i], point_light_dir[i], 0);
        color.rgb += light_contribution;
    }
    color.rgb += point_light_contribution(bec_far_pos, bec_far_color, bec_far_dir, 0);
    color.rgb += point_light_contribution(spot_far_pos1, spot_far_color1, spot_far_dir1, 1);
    color.rgb += point_light_contribution(spot_far_pos2, spot_far_color2, spot_far_dir2, 1);
    color.rgb += point_light_contribution(moon_light_pos, moon_light_color, moon_light_dir, 2);
    out_color = color;
    if (isMoon == 1) {
        out_color = texture2D(texture_1, fragTexture) + vec4(moon_light_color, 1.0f) * 0.15f;
    }

}
