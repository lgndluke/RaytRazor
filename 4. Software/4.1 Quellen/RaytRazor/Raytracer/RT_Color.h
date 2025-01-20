#ifndef RT_COLOR_H
#define RT_COLOR_H

class RT_Color {
    public:
        RT_Color();
        RT_Color(double _r, double _g, double _b, double _e);

        [[nodiscard]] double getRed() const;
        [[nodiscard]] double getGreen() const;
        [[nodiscard]] double getBlue() const;
        [[nodiscard]] double getAlpha() const;

        void setRed(double _r);
        void setGreen(double _g);
        void setBlue(double _b);
        void setAlpha(double _e);

        [[nodiscard]] double brightness() const;
        [[nodiscard]] RT_Color colorScalar(double scalar) const;
        [[nodiscard]] RT_Color addColor(const RT_Color &color) const;
        [[nodiscard]] RT_Color multiplyColor(const RT_Color &color) const;
        [[nodiscard]] RT_Color averageColor(const RT_Color &color) const;
        RT_Color clip();

    private:
        double r, g, b, extra{};
};

#endif //RT_COLOR_H
