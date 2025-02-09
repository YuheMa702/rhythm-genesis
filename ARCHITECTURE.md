# Game Architecture Framework for Rhythm Genesis

## 1. Overview
The game is divided into **four main components**:
1. **User Interface (UI) Layer** – Handles all user interactions, including menus, gameplay, and settings.
2. **Game Logic Layer** – Manages game mechanics like note spawning, scoring, timing, and input handling.
3. **Audio & Beat Map Processing Layer** – Handles audio file uploads, beat detection, and beat map storage.
4. **Data Management Layer** – Manages persistent data like beat maps, player scores, and game settings.

## 2. System Components & Flow

### 2.1 User Interface (UI) Layer
This layer manages all user interactions through Unity’s UI system.

### Main Menu UI
- Playable song list (scrolling folder of beat maps)
- **Upload Music** button (for converting songs into beat maps)
- **Beat Map Editor** button (for manual beat mapping)
- **Settings menu** (customization options, keybinds, game preferences)
- **Quit button**

### Gameplay UI
- Falling notes and horizontal judgment line
- Hit/miss indicators with animations
- Score display and combo counter
- Pause menu (resume, restart, exit)

### Results UI
- Performance summary (score, accuracy, ranking)
- Option to **play again** or **return to the song selection menu**

### Beat Map Editor UI
- Audio waveform with draggable markers for notes
- Timeline-based note placement
- Playback preview for real-time testing
- **Save button** to store beat maps in a playable format

### Upload UI
- File selector (MP3, WAV, OGG support)
- Progress bar (showing beat map generation process)
- Text input for song name
- Confirmation message upon successful save

## 2.2 Game Logic Layer
Handles **gameplay mechanics**, including note timing, scoring, and animations.

### Game Manager (Centralized controller)
- Loads beat maps and music
- Manages game state transitions (menu → gameplay → results)
- Controls difficulty scaling and visual effects

### Note Spawning System
- Reads **beat map data (JSON format)**
- Spawns notes in sync with the music
- Handles note animations and scrolling

### Scoring & Judging System
- **Hit windows**: Perfect, Excellent, Good, Bad, Miss
- Timing accuracy calculation (±10 ms error tolerance)
- **Combo & multiplier system** to reward precision

### Input Handling System
- Keyboard input for hitting notes
- Detects timing relative to the beat

## 2.3 Audio & Beat Map Processing Layer
Handles **music analysis, beat detection, and beat map generation**.

### Audio Processor (Python & librosa)
- Extracts **tempo (BPM)** and beat timings
- Detects **onsets** (strong beats) for note placement
- Converts extracted data into **JSON beat map format**

### Beat Map Manager
- Stores and retrieves **user-created & auto-generated beat maps**
- Loads beat maps into the **playable song list** dynamically

### JSON Beat Map Structure (Example)
```json
{
  "song": "example_song",
  "bpm": 120,
  "notes": [
    {"time": 0.5, "count": 2, "lanes": "1-3"},
    {"time": 1.0, "count": 1, "lanes": "2"},
    {"time": 1.5, "count": 2, "lanes": "2-4"}
  ]
}
```

## 2.4 Data Management Layer
Handles **persistent data storage**.

### Beat Map Storage
- Stores generated **beat maps in JSON format** in the game’s directory.
- Allows users to load, edit, and delete beat maps.

### Player Profile & High Scores
- Tracks personal best scores for each song.
- Saves game preferences and keybindings.

### Game Settings Storage
- Stores user preferences like **keybinds, visual settings, and audio latency correction**.

## 3. Technical Architecture

### 3.1 Tech Stack
| Component | Technology |
|-----------|------------|
| Game Engine | Unity (C#) |
| UI System | Unity UI |
| Audio Processing | Python (`librosa`) |
| Data Storage | JSON files for beat maps, Unity `PlayerPrefs` for settings |
| File Handling | Unity C# File I/O |
| Deployment | Steamworks SDK (optional for publishing) |

### 3.2 System Interaction Flow
1. **User selects a song** (preloaded or uploaded).
2. **Game loads corresponding beat map**.
3. **Notes spawn based on beat timings**.
4. **Player hits notes** with keyboard input.
5. **Scoring system evaluates timing accuracy**.
6. **Game ends and displays results**.
7. **Player can retry, select another song, or edit the beat map**.

## 4. Minimum Viable Product (MVP) Breakdown
The MVP will include:
- **Basic Gameplay** (5 preloaded songs, note hitting system, scoring)
- **Auto-Generated Beat Maps** (MP3/WAV/OGG support, JSON format)
- **Basic UI** (song selection, gameplay, results, upload button)
- **Performance Optimization** (30+ FPS, beat map loads ≤ 5 sec)

## 5. Future Features (Stretch Goals)
After achieving the MVP, we can expand:
- **Multiplayer Mode** (Online leaderboards)
- **More Customization** (User skins for notes/backgrounds)
- **Advanced Beat Map Editing** (Drag-and-drop note placement)
- **Difficulty Scaling** (Auto-adjusted note patterns for different skill levels)
