#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "UIElement.hpp"

class UILabel : public UIElement {
   private:
    sf::Text text;
    sf::Font font;
    int fontSize;

   public:
    UILabel(const std::string& label, float x, float y, int fontSize) {
        position = {x, y};

        // Load font
        if (!font.loadFromFile("../assets/fonts/golem-script.ttf")) {
            throw std::runtime_error("Failed to load font");
        }

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::White);
        text.setPosition(position);
    }

    void draw(sf::RenderWindow& window) override {
        if (visible) {
            window.draw(text);
        }
    }
};

#endif
