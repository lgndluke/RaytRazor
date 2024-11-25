#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <string>
#include <iostream>
#include "../Base_Component.h"              // Annahme: Header für Base_Component

using namespace std;

/**
 * @brief Repräsentiert eine Render-Komponente mit Verweisen auf OpenGL-Ressourcen.
 * Diese Klasse enthält Referenzen auf OpenGL-Objekte wie Geometrie und Materialien,
 * die für das Rendering eines Objekts notwendig sind.
 *
 * @author Christian Kasper
 */
class Render_Component : public Base_Component
{

    public:

        /**
         * @brief Konstruktor, der die UUIDs für das Objekt und das Material initialisiert.
         * Falls keine UUID übergeben wird, wird die UUID des Basiskomponenten-Getters verwendet.
         *
         * @param objUUID          UUID für das OpenGL-Objekt (Standard: UUID von der Base_Component)
         * @param matUUID          UUID für das OpenGL-Material (Standard: UUID von der Base_Component)
         */
        Render_Component(const string& objUUID = "", const string& matUUID = "");

        /**
         * @brief Gibt die UUID des OpenGL-Objekts zurück.
         *
         * @return string          UUID des Objekts
         */
        string getObjUUID() const;

        /**
         * @brief Setzt die UUID für das OpenGL-Objekt.
         *
         * @param newObjUUID       Neue UUID für das Objekt
         */
        void setObjUUID(const string& newObjUUID);

        /**
         * @brief Gibt die UUID des OpenGL-Materials zurück.
         *
         * @return string          UUID des Materials
         */
        string getMatUUID() const;

        /**
         * @brief Setzt die UUID für das OpenGL-Material.
         *
         * @param newMatUUID       Neue UUID für das Material
         */
        void setMatUUID(const string& newMatUUID);

        /**
         * @brief Führt das Rendering für das Objekt durch.
         * Diese Methode kann angepasst werden, um die entsprechenden OpenGL-Aufrufe zu tätigen.
         */
        void render() const;

private:

    string objUUID;      // OpenGL-Objekt-ID als UUID
    string matUUID;      // OpenGL-Material-ID als UUID

};

#endif
