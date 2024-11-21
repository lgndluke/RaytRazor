#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

//TODO Add imports here.
#include <glm/glm.hpp>

using namespace std;

/**
 * @brief //TODO
 * @author //TODO
 */
class Base_Component
{

   public:

        Base_Component();
        virtual ~Base_Component();
        //TODO Implement Getter for UUID (NOT INT! UUID -> 128 bit, INT -> only 32 bit!)
        glm::vec3 get_position();
        glm::vec3 get_rotation();
        glm::vec3 get_scale();

   private:

        //TODO implement UUID uuid (NOT INT! UUID -> 128 bit, INT -> only 32 bit!)
        glm::vec3 position {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation {0.0f, 0.0f, 0.0f};
        glm::vec3 scale {1.0f, 1.0f, 1.0f};

};

#endif
