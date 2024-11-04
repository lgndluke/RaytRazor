//
// Created by leonm on 11/1/2024.
//

#ifndef VERTEX_H
#define VERTEX_H

#include<glm/glm.hpp>

//Struktur um ein einzigen Vertex in einem face zu halten (3 Vertex = 1 Face)
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
    glm::vec3 normal;
    GLint smoothness;
    std::string object_name;
    std::string group_name;
    std::string material_name;
    std::string mtlFile_name;
};

#endif //VERTEX_H
