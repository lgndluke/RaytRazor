#include "RT_LightSource.h"

RT_LightSource::RT_LightSource(){}

Vector RT_LightSource::getLightPosition() {
    { return Vector(0, 0, 0); }
}

Color RT_LightSource::getLightColor() {
    return Color(1, 1, 1, 0);
}