#ifndef LIGHT_H
#define LIGHT_H

#include "RT_LightSource.h"
#include "RT_Vector.h"
#include "RT_Color.h"

// TODO: Kommentare schreiben

class Light : public RT_LightSource{
    public:
        Light();
        Light(Vector _position, Color _color);

        virtual Vector getLightPosition();
        virtual Color getLightColor();

    private:
        Vector position;
        Color color;
};


#endif //LIGHT_H
