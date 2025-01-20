#include "Converter.h"

void Converter::convert_to_matrix_indices(const shared_ptr<Object_Resource>& source) {
    if (!source->matrix_indices_is_empty())
        return;

    const std::vector<Indice>& input = source->get_indices();

    // Überprüfen, ob der Input leer ist
    if (input.empty()) {
        Logger::log(MessageType::WARN,
                    "Converter::convert_to_matrix_indices(): Input indices vector is empty.");
        return;
    }

    // Überprüfen, ob die Eingabe ein Vielfaches von 3 ist -> nichtmehr benötigt sollte immer normale größe haben wie in obj. datei angegebene face anzahl
    /*
    if (input.size() % 3 != 0) {
        Logger::log(MessageType::SEVERE,
                    "Converter::convert_to_matrix_indices(): Input indices size is not a multiple of 3!");
        return;
    }
    */
    try {
        // Erstellung der MatrixXu für Indizes
        nanogui::MatrixXu output(3, input.size());

        // Befüllung der Matrix
        for (size_t i = 0; i < input.size(); ++i) {
            output(0, i) = static_cast<unsigned int>(input[i].vertexIndices_v[0]); //x vertex pos
            output(1, i) = static_cast<unsigned int>(input[i].vertexIndices_v[1]); //y vertex pos
            output(2, i) = static_cast<unsigned int>(input[i].vertexIndices_v[2]); //z vertex pos
        }

        // Setzen der Matrix in `source`
        source->set_matrix_indices(output);
    } catch (const std::exception& e) {
        Logger::log(MessageType::SEVERE,
                    "Failed to set matrix indices: " + std::string(e.what()));
    }
}

void Converter::convert_to_matrix_vertices(shared_ptr<Object_Resource> source) {
    if (!source->matrix_vertices_is_empty())
        return;

    const std::vector<Vertex>& input = source->get_vertices();
    Eigen::MatrixXf output(3, input.size());

    for (size_t i = 0; i < input.size(); i++) {
        output.col(i) << input[i].position.x, input[i].position.y, input[i].position.z;
        /*
        Logger::log(MessageType::DEBUG, to_string((output.col(i)).x()));
        Logger::log(MessageType::DEBUG, to_string((output.col(i)).y()));
        Logger::log(MessageType::DEBUG, to_string((output.col(i)).z()));
        */


    }

    source->set_matrix_vertices(output);
}

void Converter::convert_to_matrix_colors(shared_ptr<Material_Resource> source) {
    if (source->get_materials().empty()) {
        Logger::log(MessageType::SEVERE,
                    "Material_Resource has no materials attached");
        return;
    }

    const std::vector<Material>& input = source->get_materials();
    Eigen::MatrixXf output(3, input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        output.col(i) << input[i].diffuse.x, input[i].diffuse.y, input[i].diffuse.z;
    }

    source->set_matrix_colors(output);
}

Eigen::Matrix4f Converter::convert_from_GLM_to_EigenMatrix(glm::mat4 source) {
    Eigen::Matrix4f eigenMatrix;

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            eigenMatrix(row, col) = source[row][col]; // glm uses column-major order
        }
    }
    return eigenMatrix;
}
