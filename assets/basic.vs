#version 300 es

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_tex_coord;
layout(location = 3) in int a_tex_index;
uniform mat3x2 view_projection;

out vec4 color;
out vec2 tex_coord;
flat out int tex_index;

void main() {
    gl_Position = vec4(view_projection * vec3(a_position, 1), 0, 1);
    tex_coord = a_tex_coord;
    tex_index = a_tex_index;
    color = a_color;
}
