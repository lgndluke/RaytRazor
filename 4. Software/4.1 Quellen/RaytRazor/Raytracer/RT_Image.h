//
// Created by leonm on 12/2/2024.
//

#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <string>
#include <vector>
#include <SDL.h>
#include <cstring>
#include "../Utility/Logger/Logger.h"

// TODO: Kommentare schreiben
class Image {
public:
    Image();
    ~Image();
    void Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer);
    void SetPixel(const int x, const int y, const double red, const double green, const double blue);
    int getXSize();
    int getYSize();
    void Display();
private:
    Uint32 ConvertColor(const double red, const double green, const double blue);
    void InitTexture();
    void computeMaxValues();
    std::vector<std::vector<double>> m_rChannel;
    std::vector<std::vector<double>> m_gChannel;
    std::vector<std::vector<double>> m_bChannel;
    int m_xSize, m_ySize;
    double m_maxRed, m_maxGreen, m_maxBlue, m_overallMax;
    SDL_Renderer *m_pRenderer;
    SDL_Texture *m_pTexture;
};

#endif //RT_IMAGE_H
