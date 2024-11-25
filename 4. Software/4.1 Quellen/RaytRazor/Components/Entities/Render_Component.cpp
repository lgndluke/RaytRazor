#include "Render_Component.h"

#include <iostream>

/**
 * @brief Konstruktor, der die UUIDs für das Objekt und das Material initialisiert.
 * Falls keine UUID übergeben wird, wird die UUID des Basiskomponenten-Getters verwendet.
 *
 * @param objUUID UUID für das OpenGL-Objekt (Standard: UUID von der Base_Component)
 * @param matUUID UUID für das OpenGL-Material (Standard: UUID von der Base_Component)
 */
Render_Component::Render_Component(const string& objUUID, const string& matUUID)
{
    // Wenn keine UUID übergeben wird, dann verwenden wir die UUID von Base_Component
    this->objUUID = objUUID;
    this->matUUID = matUUID;
}

/**
 * @brief Gibt die UUID des OpenGL-Objekts zurück.
 *
 * @return string UUID des Objekts
 */
string Render_Component::getObjUUID() const
{
    return objUUID;
}

/**
 * @brief Setzt die UUID für das OpenGL-Objekt.
 *
 * @param newObjUUID Neue UUID für das Objekt
 */
void Render_Component::setObjUUID(const string& newObjUUID)
{
    objUUID = newObjUUID;
}

/**
 * @brief Gibt die UUID des OpenGL-Materials zurück.
 *
 * @return string UUID des Materials
 */
string Render_Component::getMatUUID() const
{
    return matUUID;
}

/**
 * @brief Setzt die UUID für das OpenGL-Material.
 *
 * @param newMatUUID Neue UUID für das Material
 */
void Render_Component::setMatUUID(const string& newMatUUID)
{
    matUUID = newMatUUID;
}

/**
 * @brief Führt das Rendering für das Objekt durch.
 * Diese Methode kann angepasst werden, um die entsprechenden OpenGL-Aufrufe zu tätigen.
 */
void Render_Component::render() const
{
    // Hier könnte Code stehen, der OpenGL-Objekte rendert, basierend auf objUUID und matUUID.

    cout << "Rendering object with UUID: " << objUUID << " and material UUID: " << matUUID << endl;
}
