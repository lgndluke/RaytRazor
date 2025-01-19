#include "RT_Triangle.h"

Triangle::Triangle() {
    v1 = Vector(1, 0, 0);
    v2 = Vector(0, 1, 0);
    v3 = Vector(0, 0, 1);
    color = Color(0.5, 0.5, 0.5, 0);
    normal = Vector(0.0, 0.0, 0.0);
    normalsSet = false;
}

Triangle::Triangle(const Vector &v1_, const Vector &v2_, const Vector &v3_, const Color &color_) {
    v1 = v1_;
    v2 = v2_;
    v3 = v3_;
    color = color_;
    normal = Vector(0.0, 0.0, 0.0);
    normalsSet = false;
}

Triangle::Triangle(const Vector &v1_, const Vector &v2_, const Vector &v3_, const Vector &normal_, const Color &color_) {
    v1 = v1_;
    v2 = v2_;
    v3 = v3_;
    normal = normal_.normalize();
    color = color_;
    normalsSet = true;
}

Vector Triangle::getTriangleNormal() {
    if (!normalsSet) {
        Vector v3_1 (v3.getX() - v1.getX(), v3.getY() - v1.getY(), v3.getZ() - v1.getZ());
        const Vector v2_1 (v2.getX() - v1.getX(), v2.getY() - v1.getY(), v2.getZ() - v1.getZ());
        Vector tmp_normal = v3_1.cross(v2_1).normalize();
        normal = tmp_normal;
        return {tmp_normal.getX(), tmp_normal.getY(), tmp_normal.getZ()};
    }
    return {normal.getX(), normal.getY(), normal.getZ()};
}

double Triangle::getTriangleDistance() {
    normal = getTriangleNormal();
    distance = normal.dot(v1);
    return distance;
}

Color Triangle::getColor() { return color; }

Vector Triangle::getNormalAt(Vector point) {
    normal = getTriangleNormal();
    return normal.multiply(-1);
}

double Triangle::hit(const Ray ray) {
    Vector ray_direction = ray.getRayDirection();
    const Vector ray_origin = ray.getRayOrigin();

    if (!normalsSet) {normal = getTriangleNormal();}
    distance = getTriangleDistance();

    double a = ray_direction.dot(normal);
    if (a==0){return -1;}
    const double b = normal.dot(ray.getRayOrigin().add(normal.multiply(distance).negative()));
    const double distance_plane = -1 * b /a;

    const double q_x = ray_direction.multiply(distance_plane).getX() + ray_origin.getX();
    const double q_y = ray_direction.multiply(distance_plane).getY() + ray_origin.getY();
    const double q_z = ray_direction.multiply(distance_plane).getZ() + ray_origin.getZ();

    Vector q(q_x, q_y, q_z);
    //Condition 1
    Vector v3_1 (v3.getX() - v1.getX(), v3.getY() - v1.getY(), v3.getZ() - v1.getZ());
    const Vector vq_1 (q.getX() - v1.getX(), q.getY() - v1.getY(), q.getZ() - v1.getZ());
    const double cond1 = v3_1.cross(vq_1).dot(normal);
    //Condition 2
    Vector v2_3 (v2.getX() - v3.getX(), v2.getY() - v3.getY(), v2.getZ() - v3.getZ());
    const Vector vq_3 (q.getX() - v3.getX(), q.getY() - v3.getY(), q.getZ() - v3.getZ());
    const double cond2 = v2_3.cross(vq_3).dot(normal);
    //Condition 3
    Vector v1_2 (v1.getX() - v2.getX(), v1.getY() - v2.getY(), v1.getZ() - v2.getZ());
    const Vector vq_2 (q.getX() - v2.getX(), q.getY() - v2.getY(), q.getZ() - v2.getZ());

    if (const double cond3 = v1_2.cross(vq_2).dot(normal); cond1 >= 0 && cond2 >= 0 && cond3 >= 0) {
        return -1 * b / a;
    }
    return -1;
    }



