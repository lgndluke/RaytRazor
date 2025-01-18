#ifndef RT_SOURCE_H
#define RT_SOURCE_H

#include "RT_Vector.h"
#include "RT_Color.h"

class RT_LightSource {
public:

    RT_LightSource();

    virtual Vector getLightPosition();
    virtual Color getLightColor();
};

#endif //RT_SOURCE_H
