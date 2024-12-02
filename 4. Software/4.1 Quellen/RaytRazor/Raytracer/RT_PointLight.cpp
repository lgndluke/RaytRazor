#include "RT_PointLight.h"

PointLight::PointLight() {
    lght_color = Vector<double>{std::vector<double>{1.0, 1.0, 1.0}};
    lght_intensity = 1.0;
}

PointLight::~PointLight() {

}

bool PointLight::computeIllum(const Vector<double> &intersect, const Vector<double> &normal,
                              const std::vector<std::shared_ptr<Object>> &objects, const std::shared_ptr<Object> &object,
                              Vector<double> &color, double &intensity) {
    Vector<double> lightDirection = (lght_location - intersect).Normalized();
    Vector<double> startPoint = intersect;

    Ray lightRay = Ray(startPoint, startPoint + lightDirection);
    Vector<double> poi {3};
    Vector<double> poiNormal {3};
    Vector<double> poiColor {3};
    bool validIntersection = false;
    for (auto sceneObject : objects) {
        if (sceneObject != object) {
            validIntersection = sceneObject -> hit(lightRay, poi, poiNormal, poiColor);
        }
        if (validIntersection) break;
    }
    if (!validIntersection) {
        double angle = acos(Vector<double>::dot(normal, lightDirection));
        if (angle > 1.5708) {
            color = lght_color;
            intensity = 0.0;
            return false;
        }

        color = lght_color;
        intensity = lght_intensity * (1.0 - (angle / 1.5708));
        return true;
    }

    color = lght_color;
    intensity = 0.0;
    return false;
}


