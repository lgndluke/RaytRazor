#include "RT_LightSource.h"

RT_LightSource::RT_LightSource()= default;

Vector RT_LightSource::getLightPosition() {
    { return {0, 0, 0}; }
}

Color RT_LightSource::getLightColor() {
    return {1, 1, 1, 0};
}