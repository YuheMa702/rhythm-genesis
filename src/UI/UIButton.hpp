#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <functional>

#include "UIElement.hpp"

class UIButton : public UIElement {
   private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    int fontSize;
    std::function<void()> onClick;

   public:
    UIButton(const std::string& label, float x, float y, float width, float height, int fontSize,
             std::function<void()> callback) {
        position = {x, y};
        size = {width, height};
        onClick = callback;

        // Load font
        if (!font.loadFromFile("../assets/fonts/golem-script.ttf")) {
            throw std::runtime_error("Failed to load font");
        }

        // Button shape
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(sf::Color::Blue);

        // Button text
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + 10, y + 5);
    }

    void setText(const std::string& label) {
        text.setString(label);
    }

    void draw(sf::RenderWindow& window) override {
        if (visible) {
            window.draw(shape);
            window.draw(text);
        }
    }

    void handleEvent(const sf::Event& event) override {

        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
            if (shape.getGlobalBounds().contains(mousePos)) {
                text.setFillColor(sf::Color::Yellow);  // Change font color when hovered
            } else {
                text.setFillColor(sf::Color::White);   // Reset color when not hovered
            }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (shape.getGlobalBounds().contains(mousePos)) {
                    if (onClick)
                        onClick();  // Execute button action
                }
            }
        }
    }
};

#endif
