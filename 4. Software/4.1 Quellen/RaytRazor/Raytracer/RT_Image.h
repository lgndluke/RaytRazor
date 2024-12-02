//
// Created by leonm on 12/2/2024.
//

#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <string>
#include <vector>
#include <SDL.h>

class RT_Image {
    public:
        RT_Image();
        ~RT_Image();
        void initialize(const int width, const int height, SDL_Renderer* renderer);
        void setPx(const int x, const int y, const double red, const double green, const double blue);
        void display();
    private:
        Uint32 convertColor(const double red, const double green, const double blue);
        void initTexture();
        std::vector<std::vector<double>> img_r_channel;
        std::vector<std::vector<double>> img_g_channel;
        std::vector<std::vector<double>> img_b_channel;
        int img_width, img_height;
        SDL_Renderer* img_renderer;
        SDL_Texture* img_texture;
};

#endif //RT_IMAGE_H
