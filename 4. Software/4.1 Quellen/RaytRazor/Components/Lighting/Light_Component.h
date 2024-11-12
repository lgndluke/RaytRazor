#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

//TODO Add imports here.
#include "../Base_Component.h"

//TODO Add imports here.

using namespace std;

/**
 * @brief //TODO
 * @author //TODO
 */
class Light_Component : Base_Component
{

   public:

        //TODO add public Method definitions.

   private:

        float intensity;
        glm::vec3 color{0.0f, 0.0f, 0.0f};

};

#endif
