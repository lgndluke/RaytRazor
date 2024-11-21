#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

//TODO Add imports here.
#include "../Base_Component.h"

using namespace std;

/**
 * @brief Repräsentiert eine Render-Komponente mit Verweisen auf OpenGL-Ressourcen.
 * @author //TODO
 */
class Render_Component : Base_Component
{

    public:

        //TODO add public Method definitions.

    private:

        int objUUID;   // OpenGL-Objekt-ID /*TODO: Make this UUID and not INT!*/
        int matUUID;   // OpenGL-Material-ID /*TODO: Make this UUID and not INT!*/

};


#endif
