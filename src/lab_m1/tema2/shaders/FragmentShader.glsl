#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexture;
in vec3 fragColor;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_texture;


void main()
{
    // TODO(student): Write pixel out color
    // out_color = vec4(fragColor, 1.0f);
    out_color = vec4(abs(fragNormal), 1.0f);
    out_normal = vec4(fragNormal, 1.0f);
    out_texture = vec4(fragTexture, 0.0f, 1.0f);

}
