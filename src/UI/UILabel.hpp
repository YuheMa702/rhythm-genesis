// src/UI/UILabel.hpp
#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "UIElement.hpp"
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <functional>

class UILabel : public UIElement {
private:
    sf::Text text;
    sf::Font font;
    int fontSize;
    sf::Color normalColor = sf::Color::White;
    sf::Color hoverColor = sf::Color::Yellow;


public:
    
    UILabel(const std::string& label, float x, float y, int fontSize) : fontSize(fontSize) {
        position = {x, y};

        // Load font
        if (!font.loadFromFile("../assets/fonts/golem-script.ttf")) {
            throw std::runtime_error("Failed to load font");
        }

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(fontSize);
        text.setFillColor(normalColor);
        text.setPosition(position);
    }

    // Override setPosition to update the text position
    virtual void setPosition(float x, float y) override {
        UIElement::setPosition(x, y);
        text.setPosition(x, y);
    }

    //Added to help MainMenu find which song was selected
    const std::string getText() const {
        return text.getString().toAnsiString();
    }

    sf::FloatRect getGlobalBounds() const {
        return text.getGlobalBounds();
    }

    // Handle mouse events: change color on hover and log clicks.
    virtual void handleEvent(const sf::Event& event) override {
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
            if (text.getGlobalBounds().contains(mousePos)) {
                text.setFillColor(hoverColor);
            } else {
                text.setFillColor(normalColor);
            }
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            if (text.getGlobalBounds().contains(mousePos)) {
                spdlog::info("{} was clicked!", text.getString().toAnsiString());
            }
        } 
    }

    void draw(sf::RenderWindow& window) override {
        if (visible) {
            window.draw(text);
        }
    }
    
    sf::Vector2f getPosition() const { return position; }
};

#endif
