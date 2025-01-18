#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <vector>
#include <SDL.h>

// TODO: Kommentare schreiben
class Image {
public:
    Image();
    ~Image();
    void Initialize(int xSize, int ySize, SDL_Renderer *pRenderer);
    void SetPixel(int x, int y, double red, double green, double blue);
    [[nodiscard]] int getXSize() const;
    [[nodiscard]] int getYSize() const;
    void Display();
private:
    [[nodiscard]] Uint32 ConvertColor(double red, double green, double blue) const;
    void InitTexture();
    void computeMaxValues();
    std::vector<std::vector<double>> m_rChannel;
    std::vector<std::vector<double>> m_gChannel;
    std::vector<std::vector<double>> m_bChannel;
    int m_xSize, m_ySize;
    double m_maxRed{}, m_maxGreen{}, m_maxBlue{}, m_overallMax{};
    SDL_Renderer *m_pRenderer{};
    SDL_Texture *m_pTexture;
};

#endif //RT_IMAGE_H
