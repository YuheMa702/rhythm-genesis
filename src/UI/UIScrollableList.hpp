// src/UI/UIScrollableList.hpp
#ifndef UI_SCROLLABLE_LIST_H
#define UI_SCROLLABLE_LIST_H

#include "UIElement.hpp"
#include "UILabel.hpp"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <functional>

class UIScrollableList : public UIElement {
private:
    sf::RectangleShape background;
    std::vector<UILabel*> items;  // dynamically allocated list items
    float scrollOffset;
    float itemHeight;
    int fontSize;
    float marginX, marginY; // margins for left/right and top/bottom

public:
    // Constructor now takes optional margin values.
    UIScrollableList(float x, float y, float width, float height,
                     const std::vector<std::string>& itemStrings, int fontSize = 18,
                     float marginX = 5.0f, float marginY = 5.0f)
        : scrollOffset(0), fontSize(fontSize), marginX(marginX), marginY(marginY){
        position = {x, y};
        size = {width, height};

        // Set up background rectangle (for visual clarity)
        background.setPosition(position);
        background.setSize(size);
        background.setFillColor(sf::Color(50, 50, 50));

        // Estimate height per item (font size plus padding)
        itemHeight = fontSize + 10;

        // Create a UILabel for each song title; offset by margins.
        for (size_t i = 0; i < itemStrings.size(); i++) {
            UILabel* label = new UILabel(itemStrings[i], x + marginX, y + marginY + i * itemHeight, fontSize);
            items.push_back(label);
        }
    }
    

    ~UIScrollableList() {
        for (auto item : items) {
            delete item;
        }
    }

    // Add an item dynamically when uploading a music file.
    void addItem(const std::string& item) {
        UILabel* label = new UILabel(item, position.x + marginX, position.y + marginY + items.size() * itemHeight, fontSize);
        items.push_back(label);
    }

    void draw(sf::RenderWindow& window) override {
        if (!visible)
            return;

        // Draw the background container.
        window.draw(background);

        // Calculate visible vertical area inside the container (accounting for margins)
        float visibleTop = position.y + marginY;
        float visibleBottom = position.y + size.y - marginY;

        // Draw each item, adjusting its y position based on scrollOffset.
        for (size_t i = 0; i < items.size(); i++) {
            float newY = position.y + marginY + scrollOffset + i * itemHeight;
            // Only draw items that are within the visible area.
            if (newY + itemHeight < visibleTop || newY > visibleBottom)
                continue;
            items[i]->setPosition(position.x + marginX, newY);
            items[i]->draw(window);
        }
    }

    const std::vector<UILabel*>& getLabels() const {
        return items;
    }

    void handleEvent(const sf::Event& event) override {
        if (!visible)
            return;
        // Handle scrolling first.
        if (event.type == sf::Event::MouseWheelScrolled) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseWheelScroll.x), static_cast<float>(event.mouseWheelScroll.y));
            if (background.getGlobalBounds().contains(mousePos)) {
                scrollOffset += event.mouseWheelScroll.delta * 20;
                float totalContentHeight = items.size() * itemHeight;
                float visibleHeight = size.y - 2 * marginY;
                float maxOffset = 0;
                float minOffset = std::min(0.0f, visibleHeight - totalContentHeight);
                if (scrollOffset > maxOffset)
                    scrollOffset = maxOffset;
                if (scrollOffset < minOffset)
                    scrollOffset = minOffset;
            }
        }
        // Delegate events to each visible label.
        float visibleTop = position.y + marginY;
        float visibleBottom = position.y + size.y - marginY;
        for (size_t i = 0; i < items.size(); i++) {
            float newY = position.y + marginY + scrollOffset + i * itemHeight;
            if (newY + itemHeight < visibleTop || newY > visibleBottom)
                continue;
            items[i]->handleEvent(event);
        }
    }
};

#endif
