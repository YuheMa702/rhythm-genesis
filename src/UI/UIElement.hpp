#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <SFML/Graphics.hpp>

class UIElement {
   protected:
    sf::Vector2f position;
    sf::Vector2f size;
    bool visible = true;

   public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void handleEvent(const sf::Event& event) {}
    virtual void setPosition(float x, float y) { position = {x, y}; }
    virtual void setVisible(bool state) { visible = state; }
    virtual bool isVisible() const { return visible; }
};

#endif
