#version 330

layout(location = 0) in vec2 inPosition;

uniform mat4 Projection;
uniform mat4 Model;

void main() {
    gl_Position = Projection * Model * vec4(inPosition, 0.0, 1.0);
}
