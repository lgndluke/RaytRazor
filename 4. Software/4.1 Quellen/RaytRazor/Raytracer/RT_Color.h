#ifndef RT_COLOR_H
#define RT_COLOR_H

class Color {
    public:
        Color();
        Color(double _r, double _g, double _b, double _e);

        double getRed();
        double getGreen();
        double getBlue();
        double getAlpha();

        void setRed(double _r);
        void setGreen(double _g);
        void setBlue(double _b);
        void setAlpha(double _e);

        double brightness();
        Color colorScalar(double scalar);
        Color addColor(Color color);
        Color multiplyColor(Color color);
        Color averageColor(Color color);
        Color clip();

    private:
        double r, g, b, extra;
};

#endif //RT_COLOR_H
