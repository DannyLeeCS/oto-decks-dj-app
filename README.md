# 🎧 OtoDecks — JUCE DJ Mixing App (C++)

OtoDecks is a cross-platform DJ-style desktop application built with **C++** and the **JUCE** framework.  
It provides a dual-deck mixing workflow with waveform visualization, transport controls, and extra DJ utilities like a **playlist** and a **soundboard**.

This project focuses on real-time audio playback, GUI development, and clean OOP architecture using JUCE components.

---

## Features

### Core DJ Features
- **Dual decks** for audio playback
- **Transport controls** (play/stop, etc.)
- **Volume (gain)** control
- **Speed / playback rate** control
- **Waveform display** for track visualization
- **Load audio files** into each deck

### Utilities
- **Playlist** component to manage/select tracks
- **Soundboard** component for triggering SFX (claps, crashes, etc.)
- **Audio visualiser** for live audio feedback (visual)

---

## Tech Stack

- **C++**
- **JUCE Framework**
- Desktop GUI + audio playback
- Cross-platform builds:
  - Windows (Visual Studio 2022)
  - macOS (Xcode)
  - Linux (Makefile)

---

## Project Structure
NewProject.jucer
Assets/
    Images/
    Sounds/
Builds/
    LinuxMakefile/
        Makefile
    MacOSX/
        Info-App.plist
        RecentFilesMenuTemplate.nib
        OtoDecks.xcodeproj/
            project.pbxproj
    VisualStudio2022/
        OtoDecks_App.vcxproj
        OtoDecks_App.vcxproj.filters
        OtoDecks_App.vcxproj.user
        OtoDecks.sln
        resources.rc
        x64/
            Debug/
                App/
                    NewProject.exe.recipe
                    NewProject.ilk
                    OtoDecks_App.vcxproj.FileListAbsolute.txt
                    resources.res
JuceLibraryCode/
    BinaryData.cpp
    BinaryData.h
    BinaryData2.cpp
    include_juce_*.cpp / .mm (several)
    JuceHeader.h
    ReadMe.txt
Source/
    AudioVisualiser.cpp
    AudioVisualiser.h
    DeckGUI.cpp
    DeckGUI.h
    DJAudioPlayer.cpp
    DJAudioPlayer.h
    Main.cpp
    MainComponent.cpp
    MainComponent.h
    PlaylistComponent.cpp
    PlaylistComponent.h
    SoundBoard.cpp
    SoundBoard.h
    WaveformDisplay.cpp
    WaveformDisplay.h


### Key Modules (High-Level)

| Module | Responsibility |
|------|-----------------|
| `DJAudioPlayer` | Core audio playback logic (load, play, stop, gain, speed, position) |
| `DeckGUI` | UI controls for a single deck + links UI events to `DJAudioPlayer` |
| `WaveformDisplay` | Draws the audio waveform and playback position |
| `PlaylistComponent` | Track list UI for selecting/loading tracks |
| `SoundBoard` | Triggers short sound effects / samples |
| `AudioVisualiser` | Visual feedback based on live audio output |
| `MainComponent` | Wires decks + mixer + playlist + soundboard together |
| `Main.cpp` | JUCE app entry point |

---

## Requirements

- **JUCE** installed (Projucer included)  
- A C++ compiler with **C++17** support  
- One of:
  - **Visual Studio 2022** (Windows)
  - **Xcode** (macOS)
  - **g++ / clang + make** (Linux)

---

## How to Build & Run

### Option A (Recommended): Use Projucer (.jucer workflow)

JUCE projects are ideally built from the `.jucer` file.

1. Open `NewProject.jucer` in **Projucer**
2. Click **Save Project and Open in IDE**
3. Build & run from your IDE:
   - Windows: Visual Studio
   - macOS: Xcode
   - Linux: Makefile

> Tip: If your IDE project files break, regenerate them from `NewProject.jucer`.

---

### Option B: Build using existing IDE files

#### Windows (Visual Studio 2022)
1. Open:
   - `Builds/VisualStudio2022/OtoDecks.sln`
2. Build and run:
   - `Ctrl + F5` (run without debugger)

#### macOS (Xcode)
1. Open:
   - `Builds/MacOSX/OtoDecks.xcodeproj`
2. Build and run using Xcode Run ▶️

#### Linux (Makefile)
From the project root:
```bash
cd Builds/LinuxMakefile
make
./build/OtoDecks
```

## How to Use (Typical Flow)
Load a track to Deck 1 and Deck 2
Hit Play
Use sliders/controls to adjust:
Gain (volume)
Speed (playback rate)

Watch:
Waveform display
Audio visualiser
Use Playlist to select tracks quickly
Use Soundboard to trigger sound effects

## What This Project Demonstrates
Real-time audio playback (streamed audio)
JUCE GUI component patterns
Event-driven UI logic
Custom waveform rendering
Component-based architecture
Multi-platform build pipeline (VS/Xcode/Makefile)
Practical OOP in C++ (separation of concerns)

## Author
Developed as a C++ OOP DJ application, Goldsmiths, University of London
For academic and educational use.
Duy Anh Le (Danny Lee)
Email: dal17@student.london.ac.uk
GitHub: https://github.com/DannyLeeCS
