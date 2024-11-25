#ifndef OBJECT_RESOURCE_H
#define OBJECT_RESOURCE_H

//TODO Add imports here.
#include "../Base_Resource.h"
#include <nanogui/glutil.h>
#include <glm/glm.hpp>
#include <sstream>
#include <string>

using namespace std;

/**
 * @brief //TODO
 * @author //TODO
 */
struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
    GLint smoothness;
    string objectName;
    string groupName;
    string materialName;
    string mtlFileName;
};

/**
 * @brief //TODO
 * @author //TODO
 */
class Object_Resource : Base_Resource
{

    public:

        //TODO add public Method definitions.
        Object_Resource();
        ~Object_Resource();

    private:

        const ResourceType type = MATERIAL;
        //string path; -> Defined in Base_Resource.

};

#endif
