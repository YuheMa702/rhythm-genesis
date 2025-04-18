
// WaveformViewer.cpp
#include "WaveformViewer.hpp"
#include <iomanip>
#include <sstream>

WaveformViewer::WaveformViewer(float x, float y, float width, float height)
    : posX(x), posY(y), viewWidth(width), viewHeight(height) {}

void WaveformViewer::setBuffer(const sf::SoundBuffer& bufferRef) {
    buffer = &bufferRef;
    extractWaveform();
}

void WaveformViewer::setProgress(float prog) {
    progress = std::clamp(prog, 0.0f, 1.0f);
    currentTime = totalDuration * progress;
}

float WaveformViewer::getProgress() const {
    return progress;
}

void WaveformViewer::setOnSeekCallback(std::function<void(float)> callback) {
    onSeek = callback;
}

void WaveformViewer::setDuration(float seconds) {
    totalDuration = seconds;
}

void WaveformViewer::setCurrentTime(float seconds) {
    currentTime = seconds;
}

void WaveformViewer::extractWaveform() {
    vertices.clear();
    if (!buffer) return;
    const sf::Int16* samples = buffer->getSamples();
    std::size_t count = buffer->getSampleCount();
    unsigned sampleRate = buffer->getSampleRate() * buffer->getChannelCount();

    float centerX = viewWidth / 2.0f;

    for (std::size_t i = 0; i < count; i += 500) {
        float amplitude = static_cast<float>(samples[i]) / 32768.0f;
        float px = centerX + amplitude * (viewWidth / 2.0f);
        float py = (static_cast<float>(i) / count) * viewHeight;
        vertices.emplace_back(sf::Vector2f(px, py), sf::Color::White);
    }
}

std::string WaveformViewer::formatTime(float seconds) {
    // int mins = static_cast<int>(seconds) / 60;
    // int secs = static_cast<int>(seconds) % 60;
    // std::ostringstream ss;
    // ss << mins << ":" << std::setw(2) << std::setfill('0') << secs;
    // return ss.str();
    int mins = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    int millis = static_cast<int>((seconds - static_cast<int>(seconds)) * 1000);

    std::ostringstream ss;
    ss << mins << ":"
       << std::setw(2) << std::setfill('0') << secs << "."
       << std::setw(3) << std::setfill('0') << millis;

    return ss.str();
}

void WaveformViewer::draw(sf::RenderWindow& window) {
    if (!buffer) return;

    unsigned sampleRate = buffer->getSampleRate() * buffer->getChannelCount();
    std::size_t totalSamples = buffer->getSampleCount();

    std::size_t startSample = static_cast<std::size_t>(currentTime * sampleRate);
    std::size_t endSample = static_cast<std::size_t>((currentTime + zoomWindow) * sampleRate);
    if (endSample > totalSamples) endSample = totalSamples;

    std::vector<sf::Vertex> zoomedVertices;
    for (std::size_t i = startSample; i < endSample; i += 500) {
        float amplitude = static_cast<float>(buffer->getSamples()[i]) / 32768.0f;
        float x = posX + ((i - startSample) / static_cast<float>(sampleRate * zoomWindow)) * viewWidth;
        float y = posY + (1.0f - (amplitude + 1.0f) / 2.0f) * viewHeight;
        zoomedVertices.emplace_back(sf::Vector2f(x, y), sf::Color::White);
    }

    window.setView(window.getDefaultView());
    if (!zoomedVertices.empty()) {
        window.draw(&zoomedVertices[0], zoomedVertices.size(), sf::LineStrip);
    }

    float sliderY = posY + viewHeight + 10;
    float sliderWidth = viewWidth;
    float sliderHeight = 8;
    float handleRadius = 8;

    sf::RectangleShape track(sf::Vector2f(sliderWidth, sliderHeight));
    track.setPosition(posX, sliderY);
    track.setFillColor(sf::Color(100, 100, 100));
    window.draw(track);

    float handleX = posX + progress * sliderWidth;
    sf::CircleShape handle(handleRadius);
    handle.setOrigin(handleRadius, handleRadius);
    handle.setPosition(handleX, sliderY + sliderHeight / 2);
    handle.setFillColor(sf::Color::Yellow);
    window.draw(handle);

    sf::Font font;
    if (font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        sf::Text startTime(formatTime(currentTime), font, 14);
        startTime.setFillColor(sf::Color::White);
        startTime.setPosition(posX, sliderY + sliderHeight + 5);
        window.draw(startTime);

        sf::Text endTime(formatTime(totalDuration), font, 14);
        endTime.setFillColor(sf::Color::White);
        endTime.setPosition(posX + sliderWidth - endTime.getGlobalBounds().width, sliderY + sliderHeight + 5);
        window.draw(endTime);
    }
}

void WaveformViewer::handleEvent(const sf::Event& event) {
    if (!buffer) return;

    float sliderY = posY + viewHeight + 10;
    float sliderWidth = viewWidth;
    float handleRadius = 8;

    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseMoved) {
        bool mouseDown = (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left);
        bool mouseDrag = (event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left));

        sf::Vector2f mousePos(
            (event.type == sf::Event::MouseButtonPressed) ? event.mouseButton.x : event.mouseMove.x,
            (event.type == sf::Event::MouseButtonPressed) ? event.mouseButton.y : event.mouseMove.y
        );

        if ((mouseDown || mouseDrag) &&
            mousePos.x >= posX && mousePos.x <= posX + sliderWidth &&
            mousePos.y >= sliderY - handleRadius && mousePos.y <= sliderY + handleRadius * 3) {

            float relX = mousePos.x - posX;
            float newProgress = relX / sliderWidth;
            setProgress(newProgress);
            if (onSeek) onSeek(newProgress);
        }
    }
}

