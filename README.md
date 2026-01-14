# Kaizen: Shadow of the Regent

A 2D action platformer inspired by Nine Sols, built with C++ and SFML.

## Prerequisites

- MinGW-w64 (GCC 15.2.0+)
- CMake 4.2.1+
- SFML 3.0.2

## Project Structure

```
.
├── CMakeLists.txt          # Build configuration (explicit source list)
├── README.md               # Project docs
├── assets/                 # Sprites, audio, fonts
│   ├── sprites/
│   ├── audio/
│   └── fonts/
├── include/                # Engine/game headers
│   ├── Animation.hpp
│   ├── Attack.hpp
│   ├── Boss.hpp
│   ├── Camera.hpp
│   ├── EffectsManager.hpp
│   ├── Enemy.hpp
│   ├── Entity.hpp
│   ├── Game.hpp
│   ├── ParticleSystem.hpp
│   ├── Physics.hpp
│   ├── PixelArtGenerator.hpp
│   ├── Platform.hpp
│   ├── Player.hpp
│   └── ResourceManager.hpp
├── src/                    # Engine/game sources
│   ├── Animation.cpp
│   ├── Attack.cpp
│   ├── Boss.cpp
│   ├── Camera.cpp
│   ├── EffectsManager.cpp
│   ├── Enemy.cpp
│   ├── Entity.cpp
│   ├── Game.cpp
│   ├── HUD.cpp
│   ├── ParticleSystem.cpp
│   ├── Physics.cpp
│   ├── PixelArtGenerator.cpp
│   ├── Platform.cpp
│   ├── Player.cpp
│   ├── ResourceManager.cpp
│   └── main.cpp
└── build/                  # Build output (generated)
```

## Building the Project

### Using CMake (Windows/MinGW)

```bash
# Configure (generates build/)
cmake -S . -B build -G "MinGW Makefiles"

# Build
cmake --build build

# Run
./build/bin/Kaizen.exe
```

### Clean Build

```bash
# Remove the build directory (PowerShell)
Remove-Item -Recurse -Force build

# Reconfigure and rebuild
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

## Coding Standards

### C++ Best Practices Followed:

1. **Modern C++ (C++17)**
   - Uses smart pointers where appropriate
   - Range-based for loops
   - `const` correctness
   - RAII (Resource Acquisition Is Initialization)

2. **Code Organization**
   - Header files (.hpp) in `include/`
   - Implementation files (.cpp) in `src/`
   - Clear separation of interface and implementation

3. **Documentation**
   - Doxygen-style comments
   - Clear function and class documentation

4. **Error Handling**
   - Exception handling in main
   - Proper resource cleanup

5. **Build System**
   - CMake for cross-platform builds
   - Proper dependency management
   - Automated asset copying

6. **Version Control**
   - Comprehensive .gitignore
   - Build artifacts excluded

## Game Architecture

### Current Implementation:

- **Game Loop Pattern**: Fixed timestep game loop
- **Event-driven Input**: SFML event polling
- **Separation of Concerns**: Clear separation between game logic, rendering, and input

### Planned Features:

- Entity Component System (ECS)
- Physics engine for platforming
- Animation system
- State management (menu, gameplay, pause)
- Asset management system
- Audio system
- Particle effects

## Controls

**Movement:**
- **A/D or Arrow Keys**: Move left/right
- **Space/W/Up Arrow**: Jump (press again in air for double jump)

**Combat:**
- **J**: Light Attack (combine with W/Up for upward, S/Down for downward)
- **K**: Heavy Attack (combine with W/Up for upward, S/Down for downward)
- **P**: Parry (0.25s window, blue shield appears)

**System:**
- **ESC**: Exit game

## Quick Playtest Checklist

- Land a 3-hit light combo; verify hit sparks and HUD combo counter advance then decay.
- Hold heavy to charge, release to launch; confirm glow/charge particles and upward launcher force.
- Dash/echo step; check trail particles and short invulnerability window.
- Ledge grab then pull up; ensure no fall-through and animation swap.
- Jump → coyote/buffer test: walk off a ledge and press jump within the grace window.
- Stress particles: heavy attack into multiple enemies; ensure stable framerate.

## Development Notes

- Frame rate limited to 60 FPS
- VSync enabled for smooth rendering
- Window size: 1280x720 (adjustable in Game.hpp)

## License

[Add your license here]
