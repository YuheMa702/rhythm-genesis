#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "UIButton.hpp"
#include "UILabel.hpp"
#include "UISlider.hpp"
#include "UITextBox.hpp"
#include <vector>


class UIManager {
   private:
    std::vector<UIElement*> elements;

   public:
    void addElement(UIElement* element) { elements.push_back(element); }

    void draw(sf::RenderWindow& window) {
        for (auto& element : elements) {
            element->draw(window);
        }
    }

    void handleEvent(const sf::Event& event) {
        for (auto& element : elements) {
            element->handleEvent(event);
        }
    }
};

#endif
