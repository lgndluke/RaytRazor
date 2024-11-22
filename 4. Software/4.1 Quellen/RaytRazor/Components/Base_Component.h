#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

//TODO Add imports here.
#include <string>
#include <glm/glm.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc

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

        boost::uuids::uuid get_uuid() const;
        glm::vec3 get_position();
        glm::vec3 get_rotation();
        glm::vec3 get_scale();

   private:

        boost::uuids::uuid uuid;
        glm::vec3 position {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation {0.0f, 0.0f, 0.0f};
        glm::vec3 scale {1.0f, 1.0f, 1.0f};
};

#endif
