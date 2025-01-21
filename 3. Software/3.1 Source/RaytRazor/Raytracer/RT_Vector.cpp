#include "RT_Vector.h"

Vector::Vector() { x = y = z = 0.0; }
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

Vector Vector::subtract(const Vector &v) const { return {x - v.getX(), y - v.getY(), z - v.getZ()}; }


Vector Vector::multiply(const double scalar) const { return {x * scalar, y * scalar, z * scalar}; }

Vector Vector::multiply(const glm::mat4& matrix) const
{
    const glm::vec4 new_positions = matrix * glm::vec4(glm::vec3(x, y, z), 1.0f);
    return Vector(new_positions.x, new_positions.y, -new_positions.z);
}

bool Vector::isInitialized() const {
    if (x == 0.0 && y == 0.0 && z == 0.0) return false;
    return true;
}





