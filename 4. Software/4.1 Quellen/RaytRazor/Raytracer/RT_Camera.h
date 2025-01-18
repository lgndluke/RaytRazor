//
// Created by leonm on 11/28/2024.
//

#ifndef CAMERA_H
#define CAMERA_H
#include "RT_Vector.h"


class Camera {
public:
    Camera();
    Camera(Vector _campos, Vector _camdir, Vector _camright, Vector _camdown);

    Vector getCamPos();
    Vector getCamDir();
    Vector getCamRight();
    Vector getCamDown();
private:
    Vector campos, camdir, camright, camdown;
};
#endif //CAMERA_H
