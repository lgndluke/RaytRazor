#include "RT_Object.h"


Object::Object()= default;

Color Object::getColor(){return {0.0, 0.0, 0.0, 0};}

Vector Object::getNormalAt(Vector point) { return {0, 0, 0}; }

double Object::hit(Ray ray) { return 0; }




