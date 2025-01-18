#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include "RT_Object.h"
#include "RT_Vector.h"
#include "RT_Color.h"

// TODO: Kommentare schreiben

class Sphere : public Object {
    public:
    Sphere();
    Sphere(Vector _center, double _radius, Color _color);

    Vector getCenter();
    double getRadius();
    virtual Color getColor();
    virtual Vector getNormalAt(Vector point);
    virtual double hit(Ray ray);


    private:
        Vector center;
        double radius;
        Color color;
};


#endif //SPHERE_H
