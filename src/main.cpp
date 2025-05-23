#include <iostream>

#include <spdlog/spdlog.h>

#include <SFML/Graphics.hpp>

#include "../src/UI/MainMenu.hpp"
#include "../src/Game/MainGame.hpp"

#include "Bridge/PythonBridge.hpp"


#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>


int main() {

    // // Initialize the Python interpreter and add the "src/python" directory to sys.path
    // if (!PythonBridge::initialize("../src/Python/")) {
    //     std::cerr << "Python initialization failed!" << std::endl;
    //     return 1;
    // }

    // // Call the Python function to parse a beatmap file and get the JSON string result.
    // std::string beatmapJSON = PythonBridge::parseBeatmapFile("../assets/music/retro-game-arcade-short.mp3");
    // std::cout << "Beatmap JSON: " << beatmapJSON << std::endl;

    // // Finalize the Python interpreter before exiting
    // PythonBridge::finalize();
    // return 0;

    // Initialize logging

    spdlog::info("Starting Rhythm Genesis Main Menu...");

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktop.width;
    unsigned int screenHeight = desktop.height;
                            
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Rhythm Genesis - Main Menu", sf::Style::Close);
    window.setFramerateLimit(30);
    window.setPosition(sf::Vector2i(0, 0));
    spdlog::info("Detected screen size: {}x{}", screenWidth, screenHeight);

    MainMenu mainMenu(&window);
    mainMenu.run();
    return 0;
}


// //Use this C++ code to test your audio processing algorithm!
// #include <SFML/Audio.hpp>
// #include <SFML/System.hpp>
// #include <iostream>
// #include <vector>

// int main()
// {
//     // Load original music
//     sf::Music backgroundMusic;
//     if (!backgroundMusic.openFromFile("../assets/music/bach.ogg")) // Or something else
//     {
//         std::cerr << "Error loading background music!" << std::endl;
//         return -1;
//     }

//     // Load the beep sound buffer
//     sf::SoundBuffer beepBuffer;
//     if (!beepBuffer.loadFromFile("../assets/music/metronome-click.wav"))   // Just download some beap.wav to the assets/music folder
//     {
//         std::cerr << "Error loading beep sound!" << std::endl;
//         return -1;
//     }
//     sf::Sound beepSound;
//     beepSound.setBuffer(beepBuffer);

//     //The timestamps generated by algorithm!
//     std::vector<float> beepTimestamps = {
//     0.5341, 0.9752, 1.3932, 1.8576, 2.0434, 2.7167, 3.1579, 3.5991, 4.0403, 4.5047, 4.8994, 5.3406, 5.5496, 5.7818, 6.2229, 6.6641, 7.1285, 7.5233, 8.0573, 8.3127, 8.4288, 8.6843, 9.1254, 9.5202, 10.0078, 10.3793, 10.8669, 11.3546, 11.8886, 12.0744, 12.2834, 12.7942, 13.1889, 13.7230, 14.0713, 14.5821, 14.7679, 15.0233, 15.4645, 15.8128, 15.9753, 16.3933, 16.8112, 16.9273, 17.2989, 17.4150, 17.6472, 17.8097, 18.2741, 18.4366, 18.6688, 18.9010, 19.1797, 19.5512, 19.6905, 20.2478, 20.6890, 20.9444, 21.1302, 21.3856, 21.6178, 21.7803, 22.3608, 22.8949, 23.1039, 23.2896, 23.7308, 23.9398, 24.2416, 24.7293, 24.9382, 25.1472, 25.3794, 25.5652, 25.8206, 26.4243, 26.6797, 26.8190, 27.0977, 27.3067, 27.5156, 

//     27.6782, 27.9800, 28.1658, 28.3283, 28.6070, 28.7695, 28.9785, 29.2107, 29.4429, 29.9770, 30.2324, 30.4181, 30.6503, 30.8593, 30.9986, 31.2541, 31.5095, 31.8113, 31.9739, 32.1829, 32.3918, 32.5776, 32.8795, 33.0884, 33.3206, 33.5761, 33.8315, 34.0405, 34.2262, 34.4352, 34.6442, 34.8299, 35.1318, 35.5498, 35.7820, 36.0606, 36.3392, 36.5018, 36.7107, 36.9662, 37.4770, 37.9414, 38.0575, 38.1736, 38.4987, 38.9166, 39.3810, 39.5900, 39.8222, 40.2634, 40.6814, 40.9368, 41.1690, 41.3547, 41.5869, 42.0513, 42.4461, 42.6318, 42.7944, 43.0266, 43.2356, 43.4445, 
    
//     43.8393, 44.3269, 44.6055, 45.0235, 45.5808, 45.8362, 46.0684, 46.7882, 47.0204, 47.1597, 47.4848, 48.4600, 49.4121, 49.8300, 50.2712, 50.7124, 51.1071, 51.2929, 51.5715, 51.9895, 52.4074, 52.7790, 52.8951, 53.2434, 53.3595, 53.7310, 53.8471, 54.1025, 54.2186, 54.5437, 54.7294, 54.9384, 55.8440, 56.3316, 56.5174, 56.7263, 56.9818, 57.1675, 57.3301, 57.5855, 57.9802, 58.1660, 58.4214, 58.8858, 59.2805, 59.3966, 59.7682
//     };

//     backgroundMusic.play();  // Plays the original

//     // Use a clock to track elapsed
//     sf::Clock clock;
//     size_t nextBeepIndex = 0;

//     // Main loop: run until the background music stops
//     while (backgroundMusic.getStatus() == sf::Music::Playing)
//     {
//         // Get elapsed time in seconds
//         float elapsedTime = clock.getElapsedTime().asSeconds();

//         // Check if it's time to play the next beep sound
//         if (nextBeepIndex < beepTimestamps.size() && elapsedTime >= beepTimestamps[nextBeepIndex])
//         {
//             beepSound.play();
//             nextBeepIndex++;
//         }

//         // Sleep for a short duration to prevent busy waiting
//         sf::sleep(sf::milliseconds(1));
//     }

//     return 0;
// }
