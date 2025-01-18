#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <SDL.h>
#include "RT_Image.h"
#include "RT_Camera.h"
#include "RT_Sphere.h"
#include "RT_Light.h"
#include "RT_Plane.h"
#include "RT_LightSource.h"
#include <vector>
#include <cmath>
#include "../Utility/Logger/Logger.h"

// TODO: Kommentare schreiben

struct RGBType {
    double r;
    double g;
    double b;
};

class Scene {
    public:
        Scene();
        static bool render(Image& output);
        static void cubeScene();
        static void sphereScene();

        static Color getColorAt(Vector intersection_position, Vector intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<RT_LightSource*> light_sources, double accuracy, double ambientlight);
        static void savebmp (const char *filename, int w, int h, int dpi, const RGBType *data);
    Camera camera;
};

#endif //SCENE_H
