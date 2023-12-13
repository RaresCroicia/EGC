#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int toBeMixed;
uniform float time;
uniform int globe;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.

    vec4 color1 = texture2D(texture_1, texcoord);
    vec4 color2 = texture2D(texture_2, texcoord);
    vec4 color;
    if(globe == 1) {
        float x = texcoord.x;
        float y = texcoord.y;
        color = texture2D(texture_1, vec2(x + time / 5.f, y));
    } else if(toBeMixed == 1) {
        color = mix(color1, color2, 0.5f);
    }
    else if(toBeMixed == 0) {
        color = color1;
    }
    if(color.a < 0.5f) {
        discard;
    }
    out_color = color;

}
