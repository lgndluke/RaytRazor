
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

        [[nodiscard]] double magnitude() const;
        [[nodiscard]] Vector normalize() const;
        [[nodiscard]] Vector negative() const;
        [[nodiscard]] double dot(const Vector &v) const;
        [[nodiscard]] Vector cross(const Vector &v) const;
        [[nodiscard]] Vector add(const Vector &v) const;
        [[nodiscard]] Vector multiply(double scalar) const;

    private:
        double x;
        double y;
        double z;
};

#endif