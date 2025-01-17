#include "RT_Triangle.h"
#include <glm/glm.hpp>
#include <cmath>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>

Triangle::Triangle(const Vector<double>& _v0, const Vector<double>& _v1, const Vector<double>& _v2) {
    v0 = _v0;
    v1 = _v1;
    v2 = _v2;
}

Triangle::Triangle(const Vector<double>& _v0, const Vector<double>& _v1, const Vector<double>& _v2,
                    const Vector<double>& _n0, const Vector<double>& _n1,  const Vector<double>& _n2) {
    v0 = _v0;
    v1 = _v1;
    v2 = _v2;

    n0 = _n0;
    n1 = _n1;
    n2 = _n2;
};

Triangle::~Triangle() = default;

bool Triangle::hit(const Ray &ray, Vector<double> &point, Vector<double> &normal, Vector<double> &color) {
    const double EPSILON = 1e-8;
    Ray local_ray = obj_transform.applyTransform(ray, BWDTFORM);
    Vector<double> origin = local_ray.ray_origin;
    Vector<double> direction = local_ray.ray_ab;
    direction.Normalize();

    // Define the vertices of the triangle in object space
    Vector<double> vertex0 = obj_transform.applyTransform(v0, BWDTFORM);
    Vector<double> vertex1 = obj_transform.applyTransform(v1, BWDTFORM);
    Vector<double> vertex2 = obj_transform.applyTransform(v2, BWDTFORM);

    // Möller–Trumbore intersection algorithm
    Vector<double> edge1 = vertex1 - vertex0;
    Vector<double> edge2 = vertex2 - vertex0;
    Vector<double> h = Vector<double>::cross(direction, edge2);
    double a = Vector<double>::dot(edge1, h);

    if (closeEnough(a, 0.0)) {
        return false; // Ray is parallel to the triangle
    }

    double f = 1.0 / a;
    Vector<double> s = origin - vertex0;
    double u = f * Vector<double>::dot(s, h);

    if (u < 0.0 || u > 1.0) {
        return false; // Intersection point is outside of the triangle
    }

    Vector<double> q = Vector<double>::cross(s, edge1);
    double v = f * Vector<double>::dot(direction, q);

    if (v < 0.0 || u + v > 1.0) {
        return false; // Intersection point is outside of the triangle
    }

    double t = f * Vector<double>::dot(edge2, q);

    if (t > EPSILON) { // ray intersection
        Vector<double> poi = origin + t * direction; // point of intersection

        // Transform the point of intersection back to world space
        point = obj_transform.applyTransform(poi, FWDTFORM);

        // Calculate normal in object space and transform it to world space
        if (n0.GetNumDims() != 0) normal = (1-u-v)*n0 + u*n1 + v*n2;
        else normal = Vector<double>::cross(edge1, edge2);
        normal.Normalize();

        // Transform the normal back to world space
        normal = obj_transform.applyTransform(normal, FWDTFORM);

        color = obj_color;
        return true;
    }

    return false;
}


