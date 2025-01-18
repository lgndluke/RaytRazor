#include "RT_Color.h"

Color::Color() {
    r = 0.5;
    g = 0.5;
    b = 0.5;
}

Color::Color(double _r, double _g, double _b, double _e) {
    r = _r;
    g = _g;
    b = _b;
    extra = _e;
}

double Color::getRed() { return r; }
double Color::getGreen() { return g; }
double Color::getBlue() { return b; }
double Color::getAlpha() { return extra; }

void Color::setRed(double _r) { r = _r; }
void Color::setGreen(double _g) { g = _g; }
void Color::setBlue(double _b) { b = _b; }
void Color::setAlpha(double _e) { extra = _e; }

double Color::brightness() { return (r + g + b)/3; }
Color Color::colorScalar(double scalar) {
    return Color(r * scalar, g * scalar, b * scalar, extra);
}
Color Color::addColor(Color color) {
    return Color(r + color.getRed(), g + color.getGreen(), b + color.getBlue(), extra);
}
Color Color::multiplyColor(Color color) {
    return Color(r * color.getRed(), g * color.getGreen(), b * color.getBlue(), extra);
}

Color Color::averageColor(Color color) {
    return Color(r * color.getRed() / 2, g * color.getGreen() / 2, b * color.getBlue() / 2, extra);
}

Color Color::clip() {
    double alllight = r + g + b;
    double excesslight = alllight - 3;
    if (excesslight > 0) {
        r = r + excesslight * (r / alllight);
        g = g + excesslight * (g / alllight);
        b = b + excesslight * (b / alllight);
    }
    if (r > 1) { r = 1; }
    if (g > 1) { g = 1; }
    if (b > 1) { b = 1; }
    if (r < 0) { r = 0; }
    if (g < 0) { g = 0; }
    if (b < 0) { b = 0; }

    return Color(r, g, b, extra);
}








