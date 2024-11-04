
#ifndef OBJLOADER_H
#define OBJLOADER_H

#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Vertex.h"

static std::vector<Vertex> loadOBJ(const char* filepath) {
    // Vektoren für Dreiecke
    std::vector<glm::fvec3> vertex_pos; //v
    std::vector<glm::fvec2> vertex_texcoord; //vt
    std::vector<glm::fvec3> vertex_normal; //vn
    // Vektoren für Faces (f)
    std::vector<GLint> vertex_pos_indices; //index für v
    std::vector<GLint> vertex_texcoord_indices; //index für vt
    std::vector<GLint> vertex_normal_indices; //index für vn
    // Strukturen für MTL
    std::vector<std::string> materials;
    std::vector<GLint> material_pos_indices;
    GLint current_material = 0;

    std::string mtlFile;
    std::string objectName;

    std::vector<GLint> smoothness;
    GLint current_smoothness = 0;

    std::vector<std::string> groups;
    std::vector<GLint> group_pos_indices;
    GLint current_group = 0;

    // Strukturen zum Dateilesen
    std::stringstream ss;
    std::ifstream objFile(filepath);
    std::string line;
    std::string prefix;
    glm::vec3 tmp_vec3;
    glm::vec2 tmp_vec2;
    GLint tmp_int = 0;
    // Endprodukt
    std::vector<Vertex> vertices;

    // Falls Datei nicht geöffnet wurde, Error
    if(!objFile.is_open()) {
        std::cout<<"Datei konnte nicht ge\x94 \bffnet werden.";
        return vertices;
    }

    //Auslesen von Informationen Zeile für Zeile
    while (std::getline(objFile, line)) {
        //Prefix holen
        if(line.empty()) continue;
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "v") { // Punktkoordinaten
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vertex_pos.push_back(tmp_vec3);
        }
        else if (prefix == "vt") { // Texturen
            ss >> tmp_vec2.x >> tmp_vec2.y;
            vertex_texcoord.push_back(tmp_vec2);
        }
        else if (prefix == "vn") { // Normalen für Richtung
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vertex_normal.push_back(tmp_vec3);
        }
        else if (prefix == "f") { // Faces
            int counter = 0;
            while (ss >> tmp_int) {
                // (f) Indizes einlesen
                if (counter == 0) vertex_pos_indices.push_back(tmp_int);
                else if (counter == 1) vertex_texcoord_indices.push_back(tmp_int);
                else if (counter == 2) vertex_normal_indices.push_back(tmp_int);
                // '/' und ' ' überspringen
                if(ss.peek() == '/') {
                    counter++;
                    ss.ignore(1, '/');
                }
                else if (ss.peek() == ' ') {
                    counter++;
                    ss.ignore(1, ' ');
                }
                // Wenn v,vt,vn indizes eingelesen wurden, die nächsten einlesen
                if(counter > 2) counter = 0;
            }
            // Jeweils 3 mal, da diese informationen auf jeden block (3 jeweils) pro face zutreffen
            if (materials.empty()) materials.emplace_back("");
            material_pos_indices.emplace_back(current_material);
            material_pos_indices.emplace_back(current_material);
            material_pos_indices.emplace_back(current_material);

            smoothness.emplace_back(current_smoothness);
            smoothness.emplace_back(current_smoothness);
            smoothness.emplace_back(current_smoothness);

            if (groups.empty()) groups.emplace_back("");
            group_pos_indices.emplace_back(current_group);
            group_pos_indices.emplace_back(current_group);
            group_pos_indices.emplace_back(current_group);

        }
        else if (prefix == "s") { // Kantenglättung
            if(size_t pos = ss.str().rfind(' '); ss.str().substr(pos + 1) == "off") current_smoothness = 0;
            ss >> current_smoothness;
        }
        else if (prefix == "mtllib") { // .MTL Einbindung
            size_t pos = ss.str().rfind(' ');
            mtlFile = ss.str().substr(pos + 1);
        }
        else if (prefix == "usemtl") { // Nutzen eines MTL Objektes
            size_t pos = ss.str().rfind(' ');
            if(!materials.empty()) if(materials.back() != ss.str().substr(pos + 1)) current_material++;
            materials.push_back(ss.str().substr(pos + 1));
        }
        else if (prefix == "#") {} //Kommentar
        else if (prefix == "o") { // Objektname
            size_t pos = ss.str().rfind(' ');
            objectName = ss.str().substr(pos + 1);
        }
        else if (prefix == "g") { // Gruppenname
            size_t pos = ss.str().rfind(' ');
            if(!groups.empty()) if(groups.back() != ss.str().substr(pos + 1)) current_group++;
            groups.push_back(ss.str().substr(pos + 1));
        }
        else {
            std::cout << "Unbekannter Pr\x84 \bfix: " << prefix << std::endl;
        }
    }
    // Endprodukt zusammensetzen (Mesh)
    vertices.resize(vertex_pos_indices.size(), Vertex());

    // Alle indizes nutzen
    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].position = vertex_pos[vertex_pos_indices[i]-1];
        vertices[i].texcoord = vertex_texcoord[vertex_texcoord_indices[i]-1];
        vertices[i].normal = vertex_normal[vertex_normal_indices[i]-1];
        vertices[i].smoothness = smoothness[i];
        vertices[i].material_name = materials[material_pos_indices[i]];
        vertices[i].group_name = groups[group_pos_indices[i]];
        vertices[i].mtlFile_name = mtlFile;
        vertices[i].object_name = objectName;
        vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
    }
    // Endprodukt zurück geben
    return vertices;
}

inline void displayObj(const std::vector<Vertex>& testObj) {
    std::cout << "Object File: \n" << std::endl;
    for (unsigned int i = 0; i < testObj.size(); i++) {

        std::cout << "Face Nummer: " << i+1 << std::endl;
        std::cout << "v: " <<testObj[i].position[0] << " ";
        std::cout << testObj[i].position[1] << " ";
        std::cout << testObj[i].position[2] << " \n";
        std::cout << "vt: " <<testObj[i].texcoord[0] << " ";
        std::cout << testObj[i].texcoord[1] << " \n";
        std::cout << "vn: " <<testObj[i].normal[0] << " ";
        std::cout << testObj[i].normal[1] << " ";
        std::cout << testObj[i].normal[2] << " \n";
        std::cout << "smoothness: "<<testObj[i].smoothness << " \n";
        if(!testObj[i].material_name.empty())std::cout << "material: "<<testObj[i].material_name << " \n";
        if(!testObj[i].mtlFile_name.empty())std::cout << "mtl file: "<<testObj[i].mtlFile_name << " \n";
        if(!testObj[i].object_name.empty())std::cout << "object name: "<<testObj[i].object_name << "\n";
        if(!testObj[i].group_name.empty())std::cout << "group name: "<<testObj[i].group_name << "\n";
        std::cout << std::endl;
        if((i+1) % 3 == 0) std::cout << "\n\n";
    }
}


#endif //OBJLOADER_H
