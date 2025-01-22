//
// Created by chris on 04.12.2024.
//
#ifndef CUSTOM_LABEL_H
#define CUSTOM_LABEL_H

#include <nanogui/widget.h>
#include <nanogui/opengl.h>
#include <functional>
#include <string>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <GLFW/glfw3.h>
#include "nanogui/serializer/core.h"


using namespace std;

class Custom_Label : public nanogui::Widget {
public:

    std::function<void()> mSecondaryCallback;

    Custom_Label(Widget *parent, const std::string &caption,
          const std::string &font = "sans", int fontSize = -1);

    /// Get the label's text caption
    const std::string &caption() const { return mCaption; }
    /// Set the label's text caption
    void setCaption(const std::string &caption) { mCaption = caption; }

    /// Set the currently active font (2 are available by default: 'sans' and 'sans-bold')
    void setFont(const std::string &font) { mFont = font; }
    /// Get the currently active font
    const std::string &font() const { return mFont; }

    /// Get the label color
    nanogui::Color color() const { return mColor; }
    /// Set the label color
    void setColor(const nanogui::Color &color) { mColor = color; }

    /// Set the \ref Theme used to draw this widget
    virtual void setTheme(nanogui::Theme *theme) override;

    /// Compute the size needed to fully display the label
    virtual nanogui::Vector2i preferredSize(NVGcontext *ctx) const override;

    /// Draw the label
    virtual void draw(NVGcontext *ctx) override;

    /// Set a callback for click events
    void setCallback(const std::function<void()> &callback) { mCallback = callback; }

    /// Handle mouse button events
    virtual bool mouseButtonEvent(const nanogui::Vector2i &p, int button, bool down, int modifiers) override;

    virtual void save(nanogui::Serializer &s) const override;
    virtual bool load(nanogui::Serializer &s) override;

protected:
    std::string mCaption;
    std::string mFont;
    nanogui::Color mColor;

    /// Callback to be triggered on click
    std::function<void()> mCallback;
};

#endif // CUSTOM_LABEL_H
