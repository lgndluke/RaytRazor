#include "RT_Color.h"

RT_Color::RT_Color() {
    r = 0.5;
    g = 0.5;
    b = 0.5;
}

RT_Color::RT_Color(const double _r, const double _g, const double _b, const double _e) {
    r = _r;
    g = _g;
    b = _b;
    extra = _e;
}

double RT_Color::getRed() const { return r; }
double RT_Color::getGreen() const { return g; }
double RT_Color::getBlue() const { return b; }
double RT_Color::getAlpha() const { return extra; }

void RT_Color::setRed(const double _r) { r = _r; }
void RT_Color::setGreen(const double _g) { g = _g; }
void RT_Color::setBlue(const double _b) { b = _b; }
void RT_Color::setAlpha(const double _e) { extra = _e; }

double RT_Color::brightness() const { return (r + g + b)/3; }
RT_Color RT_Color::colorScalar(double scalar) const {
    return {r * scalar, g * scalar, b * scalar, extra};
}
RT_Color RT_Color::addColor(const RT_Color &color) const {
    return {r + color.getRed(), g + color.getGreen(), b + color.getBlue(), extra};
}
RT_Color RT_Color::multiplyColor(const RT_Color &color) const {
    return {r * color.getRed(), g * color.getGreen(), b * color.getBlue(), extra};
}

RT_Color RT_Color::averageColor(const RT_Color &color) const {
    return {r * color.getRed() / 2, g * color.getGreen() / 2, b * color.getBlue() / 2, extra};
}

RT_Color RT_Color::clip() {
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

    return {r, g, b, extra};
}








