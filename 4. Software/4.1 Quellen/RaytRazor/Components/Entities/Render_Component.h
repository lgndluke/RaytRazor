#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <string>
#include "../Base_Component.h"  // Annahme: Header für Base_Component

using namespace std;

/**
 * @brief Repräsentiert eine Render-Komponente mit Verweisen auf OpenGL-Ressourcen.
 * Diese Klasse enthält Referenzen auf OpenGL-Objekte wie Geometrie und Materialien,
 * die für das Rendering eines Objekts notwendig sind.
 *
 * @author Dein Name
 */
class Render_Component : public Base_Component
{
public:

    Render_Component(const string& objUUID = "", const string& matUUID = "");


    string getObjUUID() const;
    void setObjUUID(const string& newObjUUID);

    string getMatUUID() const;
    void setMatUUID(const string& newMatUUID);


    void render() const;

private:
    string objUUID;  // OpenGL-Objekt-ID als UUID
    string matUUID;  // OpenGL-Material-ID als UUID
};

#endif // RENDER_COMPONENT_H
