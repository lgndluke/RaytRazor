#ifndef RT_OBJECT_IMPORTER_H
#define RT_OBJECT_IMPORTER_H

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <fstream>
#include "glad/glad.h"

struct RT_Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
    GLint smoothness;
    std::string objectName;
    std::string groupName;
    std::string materialName;
    std::string mtlFileName;
};

class RT_Object_Importer {
    public:
    static std::vector<RT_Vertex> fetch_vertices(const std::string& path_to_file);
};

#endif
