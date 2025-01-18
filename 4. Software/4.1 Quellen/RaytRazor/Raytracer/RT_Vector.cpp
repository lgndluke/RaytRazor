#include "RT_Vector.h"

Vector::Vector() { x = y = z = 0; }
Vector::Vector(const double _x, const double _y, const double _z) {
    x = _x;
    y = _y;
    z = _z;
}

double Vector::getX() const { return x; }
double Vector::getY() const { return y; }
double Vector::getZ() const { return z; }

double Vector::magnitude() const { return sqrt((x * x) + (y * y) + (z * z)); }

Vector Vector::normalize() const {
    const double magnitude = this->magnitude();
    return {x / magnitude, y / magnitude, z / magnitude};
}

Vector Vector::negative() const { return {-x, -y, -z}; }

double Vector::dot(const Vector &v) const { return x * v.getX() + y * v.getY() + z * v.getZ(); }

Vector Vector::cross(const Vector &v) const { return {y * v.getZ() - z * v.getY(), z * v.getX() - x * v.getZ(), x * v.getY() - y * v.getX()}; }

Vector Vector::add(const Vector &v) const { return {x + v.getX(), y + v.getY(), z + v.getZ()}; }

Vector Vector::multiply(const double scalar) const { return {x * scalar, y * scalar, z * scalar}; }




