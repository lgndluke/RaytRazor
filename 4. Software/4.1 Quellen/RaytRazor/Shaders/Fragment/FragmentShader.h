#ifndef FRAGMENT_SHADER_H
#define FRAGMENT_SHADER_H

#include <string>

const std::string fragmentShaderSource = R"(
#version 330 core
in vec4 frag_color;
out vec4 color;
void main() {
    color = frag_color;
}
)";

#endif // FRAGMENT_SHADER_H
