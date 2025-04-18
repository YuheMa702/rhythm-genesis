
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <unordered_set>
#include "UIButton.hpp"
#include "UIElement.hpp"
#include "UIManager.hpp"
#include "UITextBox.hpp"
#include "UILabel.hpp"
#include "WaveformViewer.hpp"

class BeatMapEditor {
public:
    BeatMapEditor(sf::RenderWindow* window);
    ~BeatMapEditor();
    void run();

private:
    sf::RenderWindow* window;
    UIButton* loadButton;
    UIButton* playPauseButton;
    UIButton* quitButton;
    WaveformViewer* waveformViewer;
    UITextBox* timestampBox;
    UITextBox* laneBox;
    UIManager uiManager;

    sf::SoundBuffer buffer;
    sf::Sound sound;
    std::string audioPath;
    bool isPlaying = false;
    bool exitRequested = false;

    std::vector<std::string> beatmapStrings;
    std::unordered_set<int> noteTimestamps;

    void loadAudioFile(const std::string& path);
};
