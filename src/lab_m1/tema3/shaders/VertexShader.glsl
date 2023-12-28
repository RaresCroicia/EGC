#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;
layout(location = 3) in vec3 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 fragTexture;
out vec3 fragColor;

void main()
{
    world_position = vec3(Model * vec4(position, 1.0)).xyz;
    world_normal = normalize(mat3(Model) * normalize(normal));
    fragTexture = texture;
    fragColor = color + vec3(sin(Time), cos(Time), sin(Time)*cos(Time));
    vec4 pos = Model * vec4(position, 1.0);
    gl_Position = Projection * View * pos;
}
