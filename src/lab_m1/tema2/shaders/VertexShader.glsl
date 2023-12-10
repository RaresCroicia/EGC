#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 position;
layout(location = 3) in vec3 normal;
layout(location = 2) in vec2 texture;
layout(location = 1) in vec3 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexture;
out vec3 fragColor;

void main()
{
    // TODO(student): Send output to fragment shader
    fragPos = position;
    // fragNormal = vec3(Model * vec4(normal, 0.0));
    fragNormal = normal + vec3(sin(Time), cos(Time), sin(Time) + cos(Time));
    fragTexture = texture;
    fragColor = color + vec3(sin(Time), cos(Time), sin(Time)*cos(Time));
    // TODO(student): Compute gl_Position
    vec4 pos = Model * vec4(position, 1.0) + vec4(sin(Time), cos(Time), sin(Time) + cos(Time), 0.0);
    gl_Position = Projection * View * pos;
}
