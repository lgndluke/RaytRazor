#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include "RT_Object.h"
#include "RT_Vector.h"
#include "RT_Color.h"

class Triangle : public Object {
    public:
        Triangle();
        Triangle(Vector v1_, Vector v2_, Vector v3_, Color color_);
        Vector getTriangleNormal();
        double getTriangleDistance();
        virtual Color getColor();
        virtual Vector getNormalAt(Vector point);
        virtual double hit(Ray ray);

    private:
        Vector v1, v2, v3;
        Vector normal;
        double distance;
        Color color;
};

#endif //RT_TRIANGLE_H
