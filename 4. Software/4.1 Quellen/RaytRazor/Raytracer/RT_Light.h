#ifndef LIGHT_H
#define LIGHT_H

#include "RT_LightSource.h"
#include "RT_Vector.h"
#include "RT_Color.h"

// TODO: Kommentare schreiben

class Light : public RT_LightSource{
    public:
        Light();
        Light(const Vector &_position, const RT_Color &_color);

        Vector getLightPosition() override;
        RT_Color getLightColor() override;

    private:
        Vector position;
        RT_Color color;
};


#endif //LIGHT_H
