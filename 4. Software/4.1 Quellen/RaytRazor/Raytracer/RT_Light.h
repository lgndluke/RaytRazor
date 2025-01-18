#ifndef LIGHT_H
#define LIGHT_H

#include "RT_LightSource.h"
#include "RT_Vector.h"
#include "RT_Color.h"

// TODO: Kommentare schreiben

class Light : public RT_LightSource{
    public:
        Light();
        Light(const Vector &_position, const Color &_color);

        Vector getLightPosition() override;
        Color getLightColor() override;

    private:
        Vector position;
        Color color;
};


#endif //LIGHT_H
