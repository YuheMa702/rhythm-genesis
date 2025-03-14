// src/UI/UIFileSelector.hpp
#ifndef UI_FILE_SELECTOR_H
#define UI_FILE_SELECTOR_H

#include "UIElement.hpp"
#include "UIScrollableList.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

class UIFileSelector : public UIElement {
private:
    UIScrollableList* list;
    std::vector<std::string> files;
    std::string directory;
    std::function<void(const std::string&)> onFileSelected;
    int selectedIndex = -1;
    float marginY; // store marginY for calculations

public:
    // Allowed extensions should include the dot, e.g., ".mp3", ".wav", ".ogg"
    UIFileSelector(float x, float y, float width, float height, const std::string& directory,
                   const std::vector<std::string>& allowedExtensions,
                   int fontSize = 18, float marginX = 5.0f, float marginY = 5.0f)
        : marginY(marginY)
    {
        position = {x, y};
        size = {width, height};
        this->directory = directory;
        
        // Scan the directory for files with allowed extensions.
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                for (const auto& allowed : allowedExtensions) {
                    if (ext == allowed) {
                        files.push_back(entry.path().filename().string());
                        break;
                    }
                }
            }
        }
        
        // Create a UIScrollableList to display found files.
        list = new UIScrollableList(x, y, width, height, files, fontSize, marginX, marginY);
    }

    ~UIFileSelector() {
        delete list;
    }

    // Set a callback that is invoked when a file is selected.
    void setOnFileSelected(std::function<void(const std::string&)> callback) {
        onFileSelected = callback;
    }

    // Override handleEvent to capture mouse clicks and determine which file was clicked.
    void handleEvent(const sf::Event& event) override {
        list->handleEvent(event); // Allow list scrolling, etc.
        if (!visible)
            return;
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // Get mouse position relative to this element.
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            if (!list->isVisible())
                return;
            if (sf::FloatRect(position, size).contains(mousePos)) {
                // Calculate local Y coordinate relative to the list.
                float localY = mousePos.y - position.y;
                // Adjust for top margin.
                localY -= marginY;
                int fontSize = list->getFontSize();
                int index = static_cast<int>(localY / (fontSize + 10)); // itemHeight = fontSize+10
                if (index >= 0 && index < files.size()) {
                    selectedIndex = index;
                    if (onFileSelected) {
                        onFileSelected(files[index]);
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (visible)
            list->draw(window);
    }

    // Optionally, provide access to the currently selected file.
    std::string getSelectedFile() const {
        if (selectedIndex >= 0 && selectedIndex < files.size())
            return files[selectedIndex];
        return "";
    }
};

#endif
