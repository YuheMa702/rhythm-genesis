// WaveformViewer.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <string>
#include "UIElement.hpp"

class WaveformViewer : public UIElement {
public:
    WaveformViewer(float x, float y, float width, float height);
    void setBuffer(const sf::SoundBuffer& buffer);
    void setProgress(float progress);
    float getProgress() const;
    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event) override;
    void setOnSeekCallback(std::function<void(float)> callback);
    void setDuration(float seconds);
    void setCurrentTime(float seconds);

private:
    float posX, posY, viewWidth, viewHeight;
    float progress = 0.0f;
    float currentTime = 0.0f;
    float totalDuration = 0.0f;
    float zoomWindow = 10.0f; // 10-second window view
    const sf::SoundBuffer* buffer = nullptr;
    std::vector<sf::Vertex> vertices;
    std::function<void(float)> onSeek;

    void extractWaveform();
    std::string formatTime(float seconds);
};
