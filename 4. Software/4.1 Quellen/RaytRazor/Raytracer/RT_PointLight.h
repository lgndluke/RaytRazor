#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "RT_Light.h"

// TODO: Kommentare schreiben

class PointLight : public Light {
    public:
    PointLight();
    ~PointLight() override;

    bool computeIllum(const Vector<double> &intersect, const Vector<double> &normal,
                      const std::vector<std::shared_ptr<Object>> &objects, const std::shared_ptr<Object> &object,
                      Vector<double> &color, double &intensity) override;
};

#endif //POINTLIGHT_H
