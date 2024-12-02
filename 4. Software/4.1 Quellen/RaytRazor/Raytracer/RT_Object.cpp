#include "RT_Object.h"
#include <math.h>

#define EPSILON 1e-21f;

Object::Object(){}

Object::~Object(){}

bool Object::hit(const Ray &ray, Vector<double> &point, Vector<double> &normal, Vector<double> &color) {
    return false;
}

void Object::setTransformMatrix(const RT::GeometricTransform &transform) {
    obj_transform = transform;
}

bool Object::closeEnough(const double f1, const double f2) {
    return fabs(f1 - f2) < EPSILON;
}
