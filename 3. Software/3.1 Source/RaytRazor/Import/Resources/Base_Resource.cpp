#include "Base_Resource.h"

Base_Resource::Base_Resource(const boost::uuids::uuid uuid, const ResourceType type, const string& path)
{
    this->uuid = uuid;
    this->type = type;
    this->path = path;
}

boost::uuids::uuid Base_Resource::get_uuid() const
{
    return this->uuid;
}

ResourceType Base_Resource::get_type() const
{
    return this->type;
}

string Base_Resource::get_path() const
{
    return this->path;
}

void Base_Resource::set_path(const string& new_path)
{
    this->path = new_path;
}
