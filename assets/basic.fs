#version 300 es
precision mediump float;

out vec4 FragColor;

in vec4 color;
in vec2 tex_coord;
flat in int tex_index;

uniform sampler2D textures[ /*$max_textures*/ ];

void main() {
    if (tex_index == -1) {
        FragColor = color;
        return;
    }
    switch (tex_index) {
        /*each in textures*/ }
    FragColor = FragColor * color;
}
