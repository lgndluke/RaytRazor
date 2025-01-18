#include "RT_Triangle.h"

Triangle::Triangle() {
    v1 = Vector(1, 0, 0);
    v2 = Vector(0, 1, 0);
    v3 = Vector(0, 0, 1);
    color = Color(0.5, 0.5, 0.5, 0);
}

Triangle::Triangle(Vector v1_, Vector v2_, Vector v3_, Color color_) {
    v1 = v1_;
    v2 = v2_;
    v3 = v3_;
    color = color_;
}

Vector Triangle::getTriangleNormal() {
    Vector v3_1 (v3.getX() - v1.getX(), v3.getY() - v1.getY(), v3.getZ() - v1.getZ());
    Vector v2_1 (v2.getX() - v1.getX(), v2.getY() - v1.getY(), v2.getZ() - v1.getZ());
    normal = v3_1.cross(v2_1).normalize();
    return normal;
}

double Triangle::getTriangleDistance() {
    normal = getTriangleNormal();
    distance = normal.dot(v1);
    return distance;
}

Color Triangle::getColor() { return color; }

Vector Triangle::getNormalAt(Vector point) {
    normal = getTriangleNormal();
    return normal;
}

double Triangle::hit(Ray ray) {
        Vector ray_direction = ray.getRayDirection();
        Vector ray_origin = ray.getRayOrigin();

        normal = getTriangleNormal();
        distance = getTriangleDistance();

        double a = ray_direction.dot(normal);

        if (a == 0) {
            // ray is parallel to the plane
            return -1;
        }
        else {
            double b = normal.dot(ray.getRayOrigin().add(normal.multiply(distance).negative()));
            double distance_plane = -1 * b /a;

            double q_x = ray_direction.multiply(distance_plane).getX() + ray_origin.getX();
            double q_y = ray_direction.multiply(distance_plane).getY() + ray_origin.getY();
            double q_z = ray_direction.multiply(distance_plane).getZ() + ray_origin.getZ();

            Vector q(q_x, q_y, q_z);
            //Condition 1
            Vector v3_1 (v3.getX() - v1.getX(), v3.getY() - v1.getY(), v3.getZ() - v1.getZ());
            Vector vq_1 (q.getX() - v1.getX(), q.getY() - v1.getY(), q.getZ() - v1.getZ());
            double cond1 = v3_1.cross(vq_1).dot(normal);
            //Condition 2
            Vector v2_3 (v2.getX() - v3.getX(), v2.getY() - v3.getY(), v2.getZ() - v3.getZ());
            Vector vq_3 (q.getX() - v3.getX(), q.getY() - v3.getY(), q.getZ() - v3.getZ());
            double cond2 = v2_3.cross(vq_3).dot(normal);
            //Condition 3
            Vector v1_2 (v1.getX() - v2.getX(), v1.getY() - v2.getY(), v1.getZ() - v2.getZ());
            Vector vq_2 (q.getX() - v2.getX(), q.getY() - v2.getY(), q.getZ() - v2.getZ());
            double cond3 = v1_2.cross(vq_2).dot(normal);

            if (cond1 >= 0 && cond2 >= 0 && cond3 >= 0) {
                return -1 * b / a;
            }
            return -1;

        }
    }



