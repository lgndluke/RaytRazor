#include "Converter.h"

void Converter::convert_to_matrix_indices(Object_Resource& source) {
    if (!source.matrix_indices_is_empty())
        return;

    const std::vector<int>& input = source.get_indices();
    if (input.size() % 3 != 0) {
        Logger::log(MessageType::SEVERE,
                    "Converter::convert_to_matrix_indices(): Input indices size is not a multiple of 3!");
        return;
    }

    nanogui::MatrixXu output(3, input.size() / 3);

    for (size_t i = 0; i < input.size() / 3; ++i) {
        output.col(i) << input[i * 3], input[i * 3 + 1], input[i * 3 + 2];
    }

    source.set_matrix_indices(output);
}

void Converter::convert_to_matrix_vertices(Object_Resource& source) {
    if (!source.matrix_vertices_is_empty())
        return;

    const std::vector<Vertex>& input = source.get_vertices();
    Eigen::MatrixXf output(3, input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        output.col(i) << input[i].position.x, input[i].position.y, input[i].position.z;
    }

    source.set_matrix_vertices(output);
}

void Converter::convert_to_matrix_colors(Object_Resource& source) {
    if (!source.matrix_colors_is_empty())
        return;

    const std::vector<Vertex>& input = source.get_vertices();
    Eigen::MatrixXf output(3, input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        output.col(i) << input[i].color.x, input[i].color.y, input[i].color.z;
    }

    source.set_matrix_colors(output);
}
