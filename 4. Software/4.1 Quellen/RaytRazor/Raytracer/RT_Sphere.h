#ifndef SPHERE_H
#define SPHERE_H

#include "RT_Object.h"
#include "RT_Vector.h"
#include "RT_Color.h"
#include <cmath>

// TODO: Kommentare schreiben

class Sphere : public Object {
    public:
    Sphere();
    Sphere(const Vector &_center, double _radius, const Color &_color);

    [[nodiscard]] Vector getCenter() const;
    [[nodiscard]] double getRadius() const;
    Color getColor() override;
    Vector getNormalAt(Vector point) override;
    double hit(Ray ray) override;


    private:
        Vector center;
        double radius;
        Color color;
};


#endif //SPHERE_H
