#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOARD_SIZE 8
#define MAX_NAME_LENGTH 50
#define MAX_GAMES 100
#define HISTORY_FILE "chess_history.dat"

// Piece types
typedef enum {
    EMPTY = 0,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} PieceType;

// Piece colors
typedef enum {
    NONE = 0,
    WHITE,
    BLACK
} PieceColor;

// Piece structure
typedef struct {
    PieceType type;
    PieceColor color;
} Piece;

// Position structure
typedef struct {
    int row;
    int col;
} Position;

// Move structure
typedef struct {
    Position from;
    Position to;
    Piece captured;
} Move;

// Time control modes
typedef enum {
    BULLET,
    HYPER_BULLET,
    BLITZ,
    RAPID
} TimeControl;

// Game state
typedef struct {
    Piece board[BOARD_SIZE][BOARD_SIZE];
    PieceColor current_turn;
    int game_started;
    char white_player[MAX_NAME_LENGTH];
    char black_player[MAX_NAME_LENGTH];
    int white_score;
    int black_score;
    int move_count;
    TimeControl time_control;
    Position selected;
    int has_selection;
    Position possible_moves[64];
    int possible_moves_count;
    char white_captured[100];
    char black_captured[100];
} GameState;

// Game record for history
typedef struct {
    char white_player[MAX_NAME_LENGTH];
    char black_player[MAX_NAME_LENGTH];
    PieceColor winner;
    TimeControl time_control;
    int moves;
    time_t date;
} GameRecord;

// Chess game functions
void init_game(GameState *game);
void init_board(Piece board[BOARD_SIZE][BOARD_SIZE]);
void reset_game(GameState *game);
int is_valid_position(int row, int col);
void calculate_possible_moves(GameState *game, Position pos);
int make_move(GameState *game, Position from, Position to);
int is_move_possible(GameState *game, Position to);
char get_piece_symbol(Piece piece);
const char* get_piece_unicode(Piece piece);

// Display functions
void clear_screen(void);
void display_board(GameState *game);
void display_player_info(GameState *game);
void display_game_controls(GameState *game);
void display_menu(void);
void display_history(GameRecord *records, int count);

// History functions
int load_game_history(GameRecord *records);
void save_game_history(GameRecord *records, int count);
void add_game_to_history(GameState *game);

// Utility functions
const char* time_control_to_string(TimeControl tc);
void get_player_input(char *buffer, int max_length);
void pause_for_user(void);

#endif
