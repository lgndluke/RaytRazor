#include "Custom_Label.h"

Custom_Label::Custom_Label(Widget *parent, const std::string &caption,
                           const std::string &font, int fontSize)
    : Widget(parent), mCaption(caption), mFont(font), mColor(Color(255, 255, 255, 255)) {
    if (fontSize >= 0) {
        setFontSize(fontSize);
    }
}

void Custom_Label::setTheme(Theme *theme) {
    Widget::setTheme(theme);
    if (mTheme) {
        mColor = mTheme->mTextColor;
    }
}

Vector2i Custom_Label::preferredSize(NVGcontext *ctx) const {
    nvgFontSize(ctx, fontSize());
    nvgFontFace(ctx, mFont.c_str());
    return Vector2i(nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr), fontSize());
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

bool Custom_Label::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && down && contains(p)) {
        if (mCallback) {
            mCallback();
        }
        return true;
    }
    return Widget::mouseButtonEvent(p, button, down, modifiers);
}

void Custom_Label::save(Serializer &s) const {
    Widget::save(s);
    s.set("caption", mCaption);
    s.set("font", mFont);
    s.set("color", mColor);
}

bool Custom_Label::load(Serializer &s) {
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
        mColor = Color(255, 255, 255, 255); // Standardwert: Weiß
    }

    return true;
}

