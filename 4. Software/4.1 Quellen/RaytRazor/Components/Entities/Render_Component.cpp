#include "Render_Component.h"

Render_Component::Render_Component(const string& objUUID, const string& matUUID)
{
    this->objUUID = objUUID;
    this->matUUID = matUUID;
}

string Render_Component::getObjUUID() const
{
    return objUUID;
}

void Render_Component::setObjUUID(const string& newObjUUID)
{
    objUUID = newObjUUID;
}

string Render_Component::getMatUUID() const
{
    return matUUID;
}

void Render_Component::setMatUUID(const string& newMatUUID)
{
    matUUID = newMatUUID;
}
