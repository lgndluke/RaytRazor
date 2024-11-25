#include "Render_Component.h"

//TODO:
// - Implementieren der Render_Component::render() Methode.
// - cout aufruf in Render_Component::render() durch Logger::log() ersetzten - Logger.h in Header Datei importieren.

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

void Render_Component::render() const
{
    // Methode implementieren.
    cout << "Rendering object with UUID: " << objUUID << " and material UUID: " << matUUID << endl;
}
