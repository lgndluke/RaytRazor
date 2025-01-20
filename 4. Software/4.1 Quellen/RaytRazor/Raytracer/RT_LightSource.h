#ifndef RT_SOURCE_H
#define RT_SOURCE_H

#include "RT_Vector.h"
#include "RT_Color.h"

class RT_LightSource {
public:
    virtual ~RT_LightSource() = default;

    RT_LightSource();

    virtual Vector getLightPosition();
    virtual RT_Color getLightColor();
};

#endif //RT_SOURCE_H
