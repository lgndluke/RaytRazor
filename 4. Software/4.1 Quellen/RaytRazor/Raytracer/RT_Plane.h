#ifndef PLANE_H
#define PLANE_H

#include"RT_Object.h"
#include"RT_Vector.h"
#include"RT_Color.h"

// TODO: Kommentare schreiben

class Plane : public Object {
    public:
        Plane();
        Plane(const Vector &_normal, double _distance, const Color &_color);
        [[nodiscard]] Vector getPlaneNormal() const;
        [[nodiscard]] double getPlaneDistance() const;
        Color getColor() override;

        Vector getNormalAt(Vector point) override;
        double hit(Ray ray) override;
    private:
        Vector normal;
        double distance;
        Color color;
};

#endif //PLANE_H
