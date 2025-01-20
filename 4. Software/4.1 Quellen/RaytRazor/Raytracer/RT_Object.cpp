#include "RT_Object.h"


RT_Object::RT_Object()= default;

RT_Color RT_Object::getColor(){return {0.0, 0.0, 0.0, 0};}

Vector RT_Object::getNormalAt(Vector point) { return {0, 0, 0}; }

double RT_Object::hit(Ray ray) { return 0; }




