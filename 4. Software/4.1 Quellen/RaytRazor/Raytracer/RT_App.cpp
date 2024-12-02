//
// Created by leonm on 12/2/2024.
//

#include "RT_App.h"

RT_App::RT_App() {
    app_running = true;
    app_window = NULL;
    app_renderer = NULL;
}

bool RT_App::onInit() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;

    app_window = SDL_CreateWindow("RayTracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

    if (app_window != NULL){
        app_renderer = SDL_CreateRenderer(app_window, -1, 0);

        app_image.initialize(1280, 720, app_renderer);

        for (int x=0; x<1280; ++x){
            for (int y=0; y<720; ++y){
                double red = (static_cast<double>(x)/1280.0) * 255.0;
                double green = (static_cast<double>(y)/720.0) * 255.0;
                app_image.setPx(x, y, red, green, 0.0);
            }
        }

    }
    else return false;

    return true;
}

int RT_App::onExecute() {
    SDL_Event event;
    if (!onInit()) return -1;
    while (app_running) {
        while (SDL_PollEvent(&event) != 0) {onEvent(&event);}
        onLoop();
        onRender();
    }
    onExit();
    return 0;
}

void RT_App::onEvent(SDL_Event *event) {
    if (event->type == SDL_QUIT) app_running = false;
}

void RT_App::onLoop() {}

void RT_App::onRender() {
    SDL_SetRenderDrawColor(app_renderer, 255, 255, 255, 255);
    SDL_RenderClear(app_renderer);
    SDL_RenderPresent(app_renderer);
}

void RT_App::onExit() {
    SDL_DestroyRenderer(app_renderer);
    SDL_DestroyWindow(app_window);
    app_window = NULL;
    SDL_Quit();
}


