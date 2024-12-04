#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include <string>

const std::string vertexShaderSource = R"(
#version 330 core
uniform mat4 modelViewProj;
in vec3 position;
in vec3 color;
out vec4 frag_color;
void main() {
    frag_color = vec4(color, 1.0);
    gl_Position = modelViewProj * vec4(position, 1.0);
}
)";

#endif // VERTEX_SHADER_H
