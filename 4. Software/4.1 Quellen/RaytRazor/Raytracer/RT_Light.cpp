#include "RT_Light.h"

Light::Light() {

}

Light::~Light() {

}

bool Light::computeIllum(const Vector<double> &intersect, const Vector<double> &normal,
                         const std::vector<std::shared_ptr<Object>> &objects, const std::shared_ptr<Object> &object,
                         Vector<double> &color, double &intensity) {
    return false;
}


