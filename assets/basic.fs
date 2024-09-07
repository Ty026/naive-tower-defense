#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 tex_coord;
flat in int tex_index;

// TODO:
// we should dynamically generate this code based on the maximum
// number of texture units supported by the hardware.
// the plan is to write a pre-processing step that dynamically
// generates the switch-case code for each supported texture unit
uniform sampler2D textures[32];

void main() {
    if (tex_index == -1) {
        FragColor = vec4(1, 0, 1, 1);
        return;
    }
    switch (tex_index) {
        case 0:
        FragColor = texture(textures[0], tex_coord);
        break;
    }
}
