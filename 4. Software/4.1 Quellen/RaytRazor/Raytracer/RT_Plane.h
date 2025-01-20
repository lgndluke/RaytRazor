#ifndef PLANE_H
#define PLANE_H

#include"RT_Object.h"
#include"RT_Vector.h"
#include"RT_Color.h"

// TODO: Kommentare schreiben

class Plane : public RT_Object {
    public:
        Plane();
        Plane(const Vector &_normal, double _distance, const RT_Color &_color);
        [[nodiscard]] Vector getPlaneNormal() const;
        [[nodiscard]] double getPlaneDistance() const;
        RT_Color getColor() override;

        Vector getNormalAt(Vector point) override;
        double hit(Ray ray) override;
    private:
        Vector normal;
        double distance;
        RT_Color color;
};

#endif //PLANE_H
