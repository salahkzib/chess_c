# Royal Chess Arena - C Implementation

A beautiful terminal-based chess game written in C, featuring full chess gameplay, player management, time controls, and game history tracking.

## Features

- ♔ **Full Chess Board**: Complete 8x8 board with all pieces
- 🎯 **Move Validation**: Legal move calculation for all pieces (pawns, knights, bishops, rooks, queens, kings)
- ✨ **Highlighted Moves**: Visual indicators for selected pieces and possible moves
- 👥 **Player Management**: Customizable player names with editable profiles
- ⏱️ **Time Controls**: Multiple game modes (Bullet, Hyper Bullet, Blitz, Rapid)
- 📊 **Score Tracking**: Live score updates and captured pieces display
- 💾 **Game History**: Persistent storage of completed games
- 🎨 **Beautiful UI**: Colored terminal interface with Unicode chess pieces

## Compilation

### Using Make (Recommended)

```bash
# Compile the program
make

# Compile and run
make run

# Clean build files
make clean

# Rebuild from scratch
make rebuild
```

### Manual Compilation

```bash
gcc -Wall -Wextra -std=c99 -O2 -o chess main.c chess.c display.c history.c
```

## Running the Game

```bash
./chess
```

## How to Play

1. **Start New Game**: Select option 1 from the main menu
2. **Move Pieces**: Enter coordinates in algebraic notation (e.g., `e2`, `e4`)
   - First, select the piece to move (e.g., `e2`)
   - Then, enter the destination square (e.g., `e4`)
3. **Cancel Selection**: Type `x` to deselect a piece
4. **Quit Game**: Type `q` during gameplay to return to menu
5. **View History**: Select option 2 to see all past games

## Game Controls

- **1**: Start New Game
- **2**: View Game History
- **3**: Change Player Names
- **4**: Select Time Control
- **5**: Exit Program

## Time Control Options

- **Bullet**: 1 minute per player
- **Hyper Bullet**: 30 seconds per player
- **Blitz**: 3 minutes per player
- **Rapid**: 10 minutes per player

## File Structure

```
chess.h       - Header file with structures and declarations
main.c        - Main program loop and menu system
chess.c       - Chess game logic and move validation
display.c     - Terminal display and rendering functions
history.c     - Game history management and file I/O
Makefile      - Build configuration
```

## Technical Details

- **Language**: C (C99 standard)
- **Platform**: Linux/Unix (Windows compatible with minor modifications)
- **Dependencies**: Standard C library only
- **Storage**: Binary file format for game history

## Game Data Storage

Game history is automatically saved to `chess_history.dat` in the same directory as the executable. This file is created automatically when you complete your first game.

## Platform Notes

### Linux/Unix
- Works out of the box with colored terminal output
- Unicode chess pieces display correctly in most modern terminals

### Windows
- May require a terminal that supports ANSI color codes (Windows Terminal, ConEmu)
- For older Windows systems, modify the `clear_screen()` function in `display.c`

## Customization

You can customize various aspects of the game by modifying:

- `MAX_GAMES` in `chess.h` - Maximum number of stored games
- `MAX_NAME_LENGTH` in `chess.h` - Maximum player name length
- Color schemes in `display.c` - Terminal color codes

## Building for Different Systems

### GCC (Linux/macOS)
```bash
gcc -Wall -Wextra -std=c99 -O2 -o chess main.c chess.c display.c history.c
```

### Clang (macOS/Linux)
```bash
clang -Wall -Wextra -std=c99 -O2 -o chess main.c chess.c display.c history.c
```

### MinGW (Windows)
```bash
gcc -Wall -Wextra -std=c99 -O2 -o chess.exe main.c chess.c display.c history.c
```

## License

This is a demonstration project for educational purposes.

## Credits

Created as a C translation of the React-based Royal Chess Arena.
