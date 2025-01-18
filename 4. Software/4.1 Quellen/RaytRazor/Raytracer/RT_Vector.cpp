#include "RT_Vector.h"

Vector::Vector() { x = y = z = 0; }
Vector::Vector(double _x, double _y, double _z) {
    x = _x;
    y = _y;
    z = _z;
}

double Vector::getX() const { return x; }
double Vector::getY() const { return y; }
double Vector::getZ() const { return z; }

double Vector::magnitude() { return sqrt((x * x) + (y * y) + (z * z)); }

Vector Vector::normalize() {
    double magnitude = this->magnitude();
    return {x / magnitude, y / magnitude, z / magnitude};
}

Vector Vector::negative() { return {-x, -y, -z}; }

double Vector::dot(Vector v) { return x * v.getX() + y * v.getY() + z * v.getZ(); }

Vector Vector::cross(Vector v) { return {y * v.getZ() - z * v.getY(), z * v.getX() - x * v.getZ(), x * v.getY() - y * v.getX()}; }

Vector Vector::add(Vector v) { return {x + v.getX(), y + v.getY(), z + v.getZ()}; }

Vector Vector::multiply(double scalar) { return {x * scalar, y * scalar, z * scalar}; }




