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
#include <filesystem>
#include "../Utility/Logger/Logger.h"
#include "RT_Triangle.h"
#include "RT_Object_Importer.h"


struct RGBType {
    double r;
    double g;
    double b;
};

class RT_Scene {
    public:
        RT_Scene();
        static bool render(Image& output);
        static void cubeScene();
        static void sphereScene();
        static void previewScene();

        static RT_Color getColorAt(Vector intersection_position, Vector intersecting_ray_direction, vector<RT_Object*> scene_objects, int index_of_winning_object, vector<RT_LightSource*> light_sources, double accuracy, double ambientlight);
        static void savebmp (const char *filename, int w, int h, int dpi, const RGBType *data);
};

#endif //SCENE_H
