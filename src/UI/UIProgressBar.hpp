// src/UI/UIProgressBar.hpp
#ifndef UI_PROGRESS_BAR_H
#define UI_PROGRESS_BAR_H

#include "UIElement.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>

class UIProgressBar : public UIElement {
private:
    sf::RectangleShape background;
    sf::RectangleShape fill;
    float progress; // value from 0.0 to 1.0

public:
    UIProgressBar(float x, float y, float width, float height)
        : progress(0.0f) {
        position = {x, y};
        size = {width, height};

        background.setPosition(position);
        background.setSize(size);
        background.setFillColor(sf::Color(100, 100, 100));  // gray background

        fill.setPosition(position);
        fill.setSize({0, height});
        fill.setFillColor(sf::Color(0, 200, 0));  // green fill
    }

    void setProgress(float p) {
        progress = std::max(0.0f, std::min(p, 1.0f));
        fill.setSize({ size.x * progress, size.y });
    }

    float getProgress() const {
        return progress;
    }

    void draw(sf::RenderWindow& window) override {
        if (!visible)
            return;
        window.draw(background);
        window.draw(fill);
    }
};

#endif
