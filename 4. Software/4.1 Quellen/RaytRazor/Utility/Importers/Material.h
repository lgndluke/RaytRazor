//
// Created by leonm on 11/1/2024.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include<glm/glm.hpp>
#include<GLFW/glfw3.h>
#include<string>

//Struktur um ein einziges Material zu halten
struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat illum;
    GLfloat transparency;
    GLfloat shininess;
    std::string map_Ka;
    std::string map_Kd;
    std::string map_Ks;
    std::string map_d;
    std::string bump;
};

#endif //MATERIAL_H
