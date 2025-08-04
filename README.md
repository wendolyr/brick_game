# 🎮 BRICK GAME v2.0

A hybrid collection of classic games featuring Tetris and Snake with multiple interface options.

## 🌟 Multi-Language Architecture
### Tetris Implementation
- **Backend**: C11
- **Console Interface**: NCurses (C11)

### Snake Implementation
- **Backend**: C++20
- **Desktop Interface**: Qt6 (C++20)

## 🔑 Key Features
### Core Architecture
- **MVC Pattern** for Snake:
  - Model: Game logic
  - View: Qt6 GUI
  - Controller: Input handling and state management
- **Singleton** pattern for game state management
- **Finite State Machine** for game flow control
- 
### Tetris Features
- Classic rotation system (SRS)
- Ghost piece prediction
- Progressive difficulty system
- Score tracking with high score persistence
- Next piece preview

### Snake Features
- Configurable wall collisions
- Score-based speed scaling
- Growing length mechanic
- High score system

## 📋 Build Requirements
- Linux/macOS Terminal
- C11 compiler (GCC ≥9.3/Clang ≥12)
- C++20 compiler (GCC ≥11/Clang ≥13)
- CMake ≥3.5
- Qt6 Widgets module
- NCurses library
- GNU Make

## 🛠️ Build & Installation

```bash
# Clone repository
git clone https://github.com/yourusername/brickgame.git
cd brickgame

# Build 
make install

# Run the game
make play
```
