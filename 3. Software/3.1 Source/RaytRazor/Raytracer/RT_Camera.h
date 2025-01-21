//
// Created by leonm on 11/28/2024.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "RT_Vector.h"


class Camera {
public:
    Camera();
    Camera(const Vector &_campos, const Vector &_camdir, const Vector &_camright, const Vector &_camdown);

    [[nodiscard]] Vector getCamPos() const;
    [[nodiscard]] Vector getCamDir() const;
    [[nodiscard]] Vector getCamRight() const;
    [[nodiscard]] Vector getCamDown() const;
private:
    Vector campos, camdir, camright, camdown;
};
#endif //CAMERA_H
