#ifndef UI_SLIDER_H
#define UI_SLIDER_H

#include <algorithm>
#include "UIElement.hpp"

class UISlider : public UIElement {
private:
    sf::RectangleShape track;
    sf::RectangleShape handle;
    sf::Font font;
    sf::Text valueText;
    bool isDragging = false;
    float minValue, maxValue, currentValue;

public:
    UISlider(float x, float y, float width, float minValue, float maxValue, float initialValue) 
        : minValue(minValue), maxValue(maxValue), currentValue(initialValue) {
        position = {x, y};
        size = {width, 10};

        // Load font
        if (!font.loadFromFile("../assets/fonts/golem-script.ttf")) {
            throw std::runtime_error("Failed to load font");
        }

        // Slider track
        track.setPosition(position);
        track.setSize(size);
        track.setFillColor(sf::Color(150, 150, 150));

        // Slider handle
        handle.setSize({20, 20});
        handle.setFillColor(sf::Color::White);
        setHandlePosition();

        // Value display
        valueText.setFont(font);
        valueText.setCharacterSize(16);
        valueText.setFillColor(sf::Color::White);
        updateValueText();
    }

    void setHandlePosition() {
        float normalized = (currentValue - minValue) / (maxValue - minValue);
        float handleX = position.x + normalized * (size.x - handle.getSize().x);
        handle.setPosition(handleX, position.y - 5);
    }

    void updateValueText() {
        valueText.setString(std::to_string(static_cast<int>(currentValue)));
        valueText.setPosition(handle.getPosition().x + 25, position.y - 5);
    }

    void handleEvent(const sf::Event& event) override {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (handle.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                isDragging = true;
            }
        }
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            isDragging = false;
        }
        if (event.type == sf::Event::MouseMoved && isDragging) {
            float newX = std::clamp(static_cast<float>(event.mouseMove.x), position.x, position.x + size.x - handle.getSize().x);
            float normalized = (newX - position.x) / (size.x - handle.getSize().x);
            currentValue = minValue + normalized * (maxValue - minValue);
            handle.setPosition(newX, position.y - 5);
            updateValueText();
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (visible) {
            window.draw(track);
            window.draw(handle);
            window.draw(valueText);
        }
    }

    float getValue() const { return currentValue; }
};

#endif
