
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

class Vector {
    public:
        Vector();
        Vector(double _x, double _y, double _z);

        [[nodiscard]] double getX() const;
        [[nodiscard]] double getY() const;
        [[nodiscard]] double getZ() const;

        double magnitude();
        Vector normalize();
        Vector negative();
        double dot(Vector v);
        Vector cross(Vector v);
        Vector add(Vector v);
        Vector multiply(double scalar);

    private:
        double x;
        double y;
        double z;
};

#endif