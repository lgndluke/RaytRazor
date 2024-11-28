#include "Base_Component.h"

using namespace std;

Base_Component::Base_Component(const boost::uuids::uuid uuid, const string& name,
                               const optional<glm::vec3> position, const optional<glm::vec3> rotation,
                               const optional<glm::vec3> scale)
{
    this->uuid = uuid;
    this->name = name;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

boost::uuids::uuid Base_Component::get_uuid() const
{
    return this->uuid;
}

string Base_Component::get_name() const
{
    return this->name;
}

void Base_Component::set_name(const string& new_Name)
{
    this->name = new_Name;
}

glm::vec3 Base_Component::get_position() const
{
    if (position.has_value())
        return *position;

    return glm::vec3(0.0f, 0.0f, 0.0f);
}

void Base_Component::set_position(glm::vec3 new_Position)
{
    this->position = new_Position;
}

glm::vec3 Base_Component::get_rotation() const
{
    if (rotation.has_value())
        return *rotation;

    return glm::vec3(0.0f, 0.0f, 0.0f);
}

void Base_Component::set_rotation(glm::vec3 new_Rotation)
{
    this->rotation = new_Rotation;
}

glm::vec3 Base_Component::get_scale() const
{
    if (scale.has_value())
        return *scale;

    return glm::vec3(0, 0, 0);
}

void Base_Component::set_scale(glm::vec3 new_Scale)
{
    this->scale = new_Scale;
}