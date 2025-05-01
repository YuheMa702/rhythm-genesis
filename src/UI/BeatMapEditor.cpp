
#include "BeatMapEditor.hpp"
#include "tinyfiledialogs.h"
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>
#include <unordered_set>

BeatMapEditor::BeatMapEditor(sf::RenderWindow* window) : window(window) {
    auto winSize = window->getSize();
    float screenWidth = static_cast<float>(winSize.x);
    float screenHeight = static_cast<float>(winSize.y);

    float buttonWidth = screenWidth * 0.25f;
    float buttonHeight = screenHeight * 0.08f;
    float spacing = screenWidth * 0.05f;
    float totalWidth = buttonWidth * 3 + spacing * 2;
    float startX = (screenWidth - totalWidth) / 2.0f;
    float buttonY = screenHeight - buttonHeight - 20.0f;

    loadButton = new UIButton("Load Audio", startX, buttonY, buttonWidth, buttonHeight, 24, [this]() {
        const char* filters[] = {"*.mp3", "*.wav", "*.ogg"};
        const char* selected = tinyfd_openFileDialog("Select Audio File", "", 3, filters, "Audio Files", 0);
        if (selected) {
            audioPath = selected;
            loadAudioFile(audioPath);
        }
    });

    playPauseButton = new UIButton("Play", startX + buttonWidth + spacing, buttonY, buttonWidth, buttonHeight, 24, [this]() {
        if (!isPlaying) {
            sound.play();
            playPauseButton->setText("Pause");
            isPlaying = true;
        } else {
            sound.pause();
            playPauseButton->setText("Play");
            isPlaying = false;
        }
    });

    quitButton = new UIButton("Quit", startX + 2 * (buttonWidth + spacing), buttonY, buttonWidth, buttonHeight, 24, [this]() {
        spdlog::info("Exiting BeatMap Editor");
        exitRequested = true;
        sound.stop();  // Stop audio playback
        std::ofstream out("beatmap.json");
        out << "[\n";
        for (size_t i = 0; i < beatmapStrings.size(); ++i) {
            out << "  " << beatmapStrings[i];
            if (i < beatmapStrings.size() - 1) out << ",";
            out << "\n";
        }
        out << "]";
        out.close();
        spdlog::info("Saved beatmap.json with {} entries.", beatmapStrings.size());
    });

    waveformViewer = new WaveformViewer(screenWidth * 0.2f, screenHeight * 0.4f,
                                        screenWidth * 0.6f, screenHeight * 0.2f);

    waveformViewer->setOnSeekCallback([this](float newProgress) {
        if (buffer.getDuration().asSeconds() > 0) {
            sound.pause();
            isPlaying = false;
            playPauseButton->setText("Play");
            sf::Time newTime = sf::seconds(newProgress * buffer.getDuration().asSeconds());
            sound.setPlayingOffset(newTime);
            waveformViewer->setCurrentTime(newTime.asSeconds());
        }
    });

    float uiY = screenHeight * 0.75f;
    float labelWidth = screenWidth * 0.05f;
    float inputWidth = screenWidth * 0.15f;
    float smallButtonWidth = screenWidth * 0.1f;

    UILabel* timeLabel = new UILabel("time:", screenWidth * 0.1f, uiY, 20);
    timestampBox = new UITextBox(screenWidth * 0.1f + labelWidth, uiY, inputWidth);

    UILabel* laneLabel = new UILabel("lanes:", screenWidth * 0.1f + labelWidth + inputWidth + spacing, uiY, 20);
    laneBox = new UITextBox(screenWidth * 0.1f + labelWidth + inputWidth + spacing + labelWidth, uiY, inputWidth);

    UIButton* addNoteButton = new UIButton("Add", screenWidth * 0.6f, uiY, smallButtonWidth, buttonHeight * 0.5f, 20, [this]() {
        std::string tsStr = timestampBox->getText();
        std::string lanesStr = laneBox->getText();

        try {
            float timestamp = std::stof(tsStr);
            int tsInt = static_cast<int>(timestamp * 1000); // round to ms precision
            if (noteTimestamps.find(tsInt) != noteTimestamps.end()) {
                spdlog::warn("Note at {}s already exists, skipping.", timestamp);
                return;
            }

            std::vector<int> lanes;
            std::stringstream ss(lanesStr);
            std::string token;
            while (std::getline(ss, token, ',')) {
                lanes.push_back(std::stoi(token));
            }

            std::ostringstream oss;
            oss << "{ \"time\": " << timestamp << ", \"num_notes\": " << lanes.size() << ", \"lanes\": [";
            for (size_t i = 0; i < lanes.size(); ++i) {
                oss << lanes[i];
                if (i < lanes.size() - 1) oss << ", ";
            }
            oss << "] }";

            beatmapStrings.push_back(oss.str());
            noteTimestamps.insert(tsInt);
            spdlog::info("Added note at {}s with {} notes.", timestamp, lanes.size());
        } catch (const std::exception& e) {
            spdlog::error("Failed to parse note input: {}", e.what());
        }
    });

    UIButton* deleteNoteButton = new UIButton("Delete", screenWidth * 0.71f, uiY, smallButtonWidth, buttonHeight * 0.5f, 20, [this]() {
        std::string tsStr = timestampBox->getText();
        try {
            float timestamp = std::stof(tsStr);
            int tsInt = static_cast<int>(timestamp * 1000);
            if (noteTimestamps.erase(tsInt) > 0) {
                beatmapStrings.erase(std::remove_if(beatmapStrings.begin(), beatmapStrings.end(), [tsInt](const std::string& entry) {
                    return entry.find(std::to_string(tsInt / 1000.0f)) != std::string::npos;
                }), beatmapStrings.end());
                spdlog::info("Deleted notes at {}s", timestamp);
            }
        } catch (...) {
            spdlog::warn("Invalid timestamp for delete");
        }
    });

    UIButton* showJsonButton = new UIButton("Show JSON", screenWidth * 0.82f, uiY, smallButtonWidth, buttonHeight * 0.5f, 20, [this]() {
        spdlog::info("Current beatmap JSON:");
        spdlog::info("[");
        for (size_t i = 0; i < beatmapStrings.size(); ++i) {
            spdlog::info("  {}{}", beatmapStrings[i], (i < beatmapStrings.size() - 1 ? "," : ""));
        }
        spdlog::info("]");
    });

    uiManager.addElement(loadButton);
    uiManager.addElement(playPauseButton);
    uiManager.addElement(quitButton);
    uiManager.addElement(waveformViewer);
    uiManager.addElement(timeLabel);
    uiManager.addElement(timestampBox);
    uiManager.addElement(laneLabel);
    uiManager.addElement(laneBox);
    uiManager.addElement(addNoteButton);
    uiManager.addElement(deleteNoteButton);
    uiManager.addElement(showJsonButton);
}

BeatMapEditor::~BeatMapEditor() {
    sound.stop();  // Ensure sound is stopped before cleanup
    delete loadButton;
    delete playPauseButton;
    delete quitButton;
    delete waveformViewer;
    delete timestampBox;
    delete laneBox;
}

void BeatMapEditor::loadAudioFile(const std::string& path) {
    if (buffer.loadFromFile(path)) {
        sound.setBuffer(buffer);
        waveformViewer->setBuffer(buffer);
        waveformViewer->setDuration(buffer.getDuration().asSeconds());
        waveformViewer->setCurrentTime(0.0f);
        spdlog::info("Audio loaded: {}", path);
    } else {
        spdlog::error("Failed to load audio: {}", path);
    }
}

void BeatMapEditor::run() {
    while (window->isOpen() && !exitRequested) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            uiManager.handleEvent(event);
        }

        if (isPlaying) {
            float currentSec = sound.getPlayingOffset().asSeconds();
            float totalSec = buffer.getDuration().asSeconds();
            float progress = currentSec / totalSec;
            waveformViewer->setProgress(progress);
            waveformViewer->setCurrentTime(currentSec);
        }

        window->clear(sf::Color::Black);
        uiManager.draw(*window);
        window->display();
    }
}
