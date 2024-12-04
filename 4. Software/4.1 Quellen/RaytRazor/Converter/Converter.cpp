#include "Converter.h"

nanogui::MatrixXu Convert_to_Indices(const Object_Resource& source){

    vector<int> input = source.get_indices();
    nanogui::MatrixXu output(3,input.size()/3);

    for (int i = 0; i < input.size()/3; i++) {
        output.col(i) << input[i], input[i+1], input[i+2];
    }
}

nanogui::MatrixXf Convert_to_Vertices(const Object_Resource& source){

    vector<Vertex> input = source.get_vertices();
    nanogui::MatrixXf output(3,input.size()/3);

    for (int i = 0; i < input.size(); ++i) {
        output.col(i) << input[i].position.x, input[i].position.y, input[i].position.z;
    }
}

nanogui::MatrixXf Convert_to_Color(const Object_Resource& source){
    vector<Vertex> input = source.get_vertices();
    nanogui::MatrixXf output(3,input.size()/3);

    for (int i = 0; i < input.size(); ++i) {
        output.col(i) << input[i].color.x, input[i].color.y, input[i].color.z;
    }
}