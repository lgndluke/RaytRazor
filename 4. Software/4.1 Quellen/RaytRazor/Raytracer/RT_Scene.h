#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <SDL.h>
#include "RT_Image.h"
#include "RT_Camera.h"
#include "RT_Sphere.h"
#include "RT_Plane.h"
#include "RT_PointLight.h"

// TODO: Kommentare schreiben

class Scene {
    public:
        Scene();
        bool render(Image& output);
    private:
        Camera camera;

        std::vector<std::shared_ptr<Object>> objects;
        std::vector<std::shared_ptr<Light>> lights;
};

#endif //SCENE_H
