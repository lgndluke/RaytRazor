#include "Base_Component.h"

using namespace std;

Base_Component::Base_Component()
{
    uuid = boost::uuids::random_generator()();
}

boost::uuids::uuid Base_Component::get_uuid() const
{
    return uuid;
}

glm::vec3 Base_Component::get_position() const
{
    return position;
}

glm::vec3 Base_Component::get_rotation() const
{
    return rotation;
}

glm::vec3 Base_Component::get_scale() const
{
    return scale;
}