#version 300 es

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec4 a_color;

void main() {
    gl_Position = vec4(a_position, 0.0, 1.0);
}
