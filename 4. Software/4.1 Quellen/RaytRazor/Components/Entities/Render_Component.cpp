#include "Render_Component.h"

Render_Component::Render_Component(const boost::uuids::uuid uuid,
                                   const string& name,
                                   const optional<glm::vec3> position,
                                   const optional<glm::vec3> rotation,
                                   const optional<glm::vec3> scale,
                                   const boost::uuids::uuid object_UUID,
                                   const boost::uuids::uuid material_UUID)
                                   : Base_Component(uuid, name, position, rotation, scale)
{
    this->object_UUID = object_UUID;
    this->material_UUID = material_UUID;
}

boost::uuids::uuid Render_Component::get_object_UUID() const
{
    return this->object_UUID;
}

void Render_Component::set_object_UUID(const boost::uuids::uuid new_UUID)
{
    this->object_UUID = new_UUID;
}

boost::uuids::uuid Render_Component::get_material_UUID() const
{
    return this->material_UUID;
}

void Render_Component::set_material_UUID(const boost::uuids::uuid new_UUID)
{
    this->material_UUID = new_UUID;
}