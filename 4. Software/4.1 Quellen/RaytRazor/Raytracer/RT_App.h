//
// Created by leonm on 12/2/2024.
//

#ifndef RT_APP_H
#define RT_APP_H

#include <SDL.h>
#include "RT_Image.h"

class RT_App {
    public:
        RT_App();
        int onExecute();
        bool onInit();
        void onEvent(SDL_Event* e);
        void onLoop();
        void onRender();
        void onExit();
    private:
        RT_Image app_image;
        bool app_running;
        SDL_Window* app_window;
        SDL_Renderer* app_renderer;
};

#endif //RT_APP_H
