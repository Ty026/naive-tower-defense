#version 300 es

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec4 a_color;
uniform mat3x2 view_projection;

void main() {
    gl_Position = vec4(view_projection * vec3(a_position, 1), 0, 1);
}
