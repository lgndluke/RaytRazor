#include "Custom_Label.h"

Custom_Label::Custom_Label(Widget *parent, const std::string &caption,
                           const std::string &font, int fontSize)
    : Widget(parent), mCaption(caption), mFont(font), mColor(nanogui::Color(255, 255, 255, 255)) {
    if (fontSize >= 0) {
        setFontSize(fontSize);
    }
}

void Custom_Label::setTheme(nanogui::Theme *theme) {
    Widget::setTheme(theme);
    if (mTheme) {
        mColor = mTheme->mTextColor;
    }
}

nanogui::Vector2i Custom_Label::preferredSize(NVGcontext *ctx) const {
    nvgFontSize(ctx, fontSize());
    nvgFontFace(ctx, mFont.c_str());
    return nanogui::Vector2i(nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr), fontSize());
}

void Custom_Label::draw(NVGcontext *ctx) {
    Widget::draw(ctx);

    nvgFontFace(ctx, mFont.c_str());
    nvgFontSize(ctx, static_cast<float>(fontSize()));
    nvgFillColor(ctx, mColor);

    if (mFixedSize.x() > 0) {
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgTextBox(ctx, mPos.x(), mPos.y(), mFixedSize.x(), mCaption.c_str(), nullptr);
    } else {
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        nvgText(ctx, mPos.x(), mPos.y() + mSize.y() * 0.5f, mCaption.c_str(), nullptr);
    }
}

bool Custom_Label::mouseButtonEvent(const nanogui::Vector2i &p, int button, bool down, int modifiers) {
    if (down && contains(p)) { // Nur wenn der Button gedrückt wurde und der Punkt innerhalb des Widgets liegt
        if (button == GLFW_MOUSE_BUTTON_LEFT) { // Linke Maustaste
            if (mCallback) {
                mCallback(); // Linke Maustasten-Callback
            }
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) { // Rechte Maustaste
            if (mSecondaryCallback) {
                mSecondaryCallback(); // Rechte Maustasten-Callback
            }
        }
        return true; // Event verarbeitet
    }
    return Widget::mouseButtonEvent(p, button, down, modifiers); // An den Standard-Handler weitergeben
}

void Custom_Label::save(nanogui::Serializer &s) const {
    Widget::save(s);
    s.set("caption", mCaption);
    s.set("font", mFont);
    s.set("color", mColor);
}

bool Custom_Label::load(nanogui::Serializer &s) {
    if (!Widget::load(s)) {
        return false;
    }

    // Überprüfen, ob die jeweiligen Felder existieren, bevor sie geladen werden
    if (!s.get("caption", mCaption)) {
        mCaption = ""; // Standardwert
    }

    if (!s.get("font", mFont)) {
        mFont = "sans"; // Standardwert
    }

    if (!s.get("color", mColor)) {
        mColor = nanogui::Color(255, 255, 255, 255); // Standardwert: Weiß
    }

    return true;
}
