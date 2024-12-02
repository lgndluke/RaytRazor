#include "RT_Image.h"
#include <fstream>

RT_Image::RT_Image() {
    img_width = 0;
    img_height = 0;
    img_texture = NULL;
}

RT_Image::~RT_Image() {
    if (img_texture != NULL) SDL_DestroyTexture(img_texture);
}

void RT_Image::initialize(const int width, const int height, SDL_Renderer *renderer) {
    img_r_channel.resize(width, std::vector<double>(height, 0.0));
    img_g_channel.resize(width, std::vector<double>(height, 0.0));
    img_b_channel.resize(width, std::vector<double>(height, 0.0));

    img_width = width;
    img_height = height;
    img_renderer = renderer;
    initTexture();
}

void RT_Image::setPx(const int x, const int y, const double red, const double green, const double blue) {
    img_r_channel[x][y] = red;
    img_g_channel[x][y] = green;
    img_b_channel[x][y] = blue;
}

void RT_Image::display() {
    Uint32 *tempPixels = new Uint32[img_width * img_height];
    memset(tempPixels, 0, img_width * img_height * sizeof(Uint32));
    for (int x=0; x<img_width; x++) {
        for (int y=0; y<img_height; y++) {
            tempPixels[(y*img_width)+x] = convertColor(img_r_channel.at(x).at(y), img_g_channel.at(x).at(y), img_b_channel.at(x).at(y));
        }
    }
    SDL_UpdateTexture(img_texture, NULL, tempPixels, img_width * sizeof(Uint32));
    delete[] tempPixels;
    SDL_Rect srcRect, bounds;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 1280;
    srcRect.h = 720;
    bounds = srcRect;
    SDL_RenderCopy(img_renderer, img_texture, &srcRect, &bounds);
}

void RT_Image::initTexture() {
    Uint32 rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif
    if (img_texture != NULL) SDL_DestroyTexture(img_texture);
    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, img_width, img_height, 32, rmask, gmask, bmask, amask);
    img_texture = SDL_CreateTextureFromSurface(img_renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

Uint32 RT_Image::convertColor(const double red, const double green, const double blue)
{
    unsigned char r = static_cast<unsigned char>(red);
    unsigned char g = static_cast<unsigned char>(green);
    unsigned char b = static_cast<unsigned char>(blue);

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Uint32 pixelColor = (r << 24) + (g << 16) + (b << 8) + 255;
    #else
        Uint32 pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
    #endif

    return pixelColor;
}