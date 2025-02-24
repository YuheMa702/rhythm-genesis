#ifndef UI_TEXTBOX_H
#define UI_TEXTBOX_H

#include "UIElement.hpp"
#include <string>

class UITextBox : public UIElement {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Font font;
    std::string inputString;
    bool isActive = false;

public:
    UITextBox(float x, float y, float width) {
        position = {x, y};
        size = {width, 30};

        // Load font
        if (!font.loadFromFile("../assets/fonts/golem-script.ttf")) {
            throw std::runtime_error("Failed to load font");
        }

        // Text box
        box.setPosition(position);
        box.setSize(size);
        box.setFillColor(sf::Color::Black);
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::White);

        // Display text
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setPosition(position.x + 5, position.y + 5);
    }

    void handleEvent(const sf::Event& event) override {
        if (event.type == sf::Event::MouseButtonPressed) {
            isActive = box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
            box.setOutlineColor(isActive ? sf::Color::Cyan : sf::Color::White);
        }
        if (isActive && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8 && !inputString.empty()) { // Backspace
                inputString.pop_back();
            } else if (event.text.unicode < 128 && event.text.unicode > 31) { // Printable chars
                inputString += static_cast<char>(event.text.unicode);
            }
            text.setString(inputString);
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (visible) {
            window.draw(box);
            window.draw(text);
        }
    }

    std::string getText() const { return inputString; }
};

#endif
