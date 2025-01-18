#ifndef RT_COLOR_H
#define RT_COLOR_H

class Color {
    public:
        Color();
        Color(double _r, double _g, double _b, double _e);

        [[nodiscard]] double getRed() const;
        [[nodiscard]] double getGreen() const;
        [[nodiscard]] double getBlue() const;
        [[nodiscard]] double getAlpha() const;

        void setRed(double _r);
        void setGreen(double _g);
        void setBlue(double _b);
        void setAlpha(double _e);

        [[nodiscard]] double brightness() const;
        [[nodiscard]] Color colorScalar(double scalar) const;
        [[nodiscard]] Color addColor(const Color &color) const;
        [[nodiscard]] Color multiplyColor(const Color &color) const;
        [[nodiscard]] Color averageColor(const Color &color) const;
        Color clip();

    private:
        double r, g, b, extra{};
};

#endif //RT_COLOR_H
