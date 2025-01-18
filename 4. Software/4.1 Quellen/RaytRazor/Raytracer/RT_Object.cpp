#include "RT_Object.h"


Object::Object(){}

Color Object::getColor(){return Color(0.0, 0.0, 0.0, 0);}

Vector Object::getNormalAt(Vector point) { return Vector(0, 0, 0); }

double Object::hit(Ray ray) { return 0; }




