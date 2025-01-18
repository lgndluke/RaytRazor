#ifndef PLANE_H
#define PLANE_H

#include"RT_Object.h"
#include"RT_Vector.h"
#include"RT_Color.h"

// TODO: Kommentare schreiben

class Plane : public Object {
    public:
        Plane();
        Plane(Vector _normal, double _distance, Color _color);
        Vector getPlaneNormal();
        double getPlaneDistance();
        virtual Color getColor();

        virtual Vector getNormalAt(Vector point);
        virtual double hit(Ray ray);
    private:
        Vector normal;
        double distance;
        Color color;
};

#endif //PLANE_H
