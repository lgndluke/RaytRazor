#ifndef MATERIAL_RESOURCE_H
#define MATERIAL_RESOURCE_H

#include "../Base_Resource.h"
#include <glm/glm.hpp>
#include <nanogui/glutil.h>
#include <sstream>
#include <string>

using namespace std;

/**
 * @brief //TODO
 * @author //TODO
 */
struct Material
{
    string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLfloat illumination;
    GLfloat transparency;
    GLfloat shininess;
    string map_Ka;
    string map_Kd;
    string map_Ks;
    string map_d;
    string bump;
};

/**
 * @brief //TODO
 * @author //TODO
 */
class Material_Resource : Base_Resource
{

    public:

        //TODO add public Method definitions.
        Material_Resource();
        ~Material_Resource();

    private:

        const ResourceType type = MATERIAL;
        //string path; -> Defined in Base_Resource.

};

#endif
