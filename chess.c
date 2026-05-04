#include "chess.h"

void init_game(GameState *game) {
    strcpy(game->white_player, "Player 1");
    strcpy(game->black_player, "Player 2");
    game->white_score = 0;
    game->black_score = 0;
    game->move_count = 0;
    game->time_control = BLITZ;
    game->game_started = 0;
    game->current_turn = WHITE;
    game->has_selection = 0;
    game->possible_moves_count = 0;
    strcpy(game->white_captured, "");
    strcpy(game->black_captured, "");
    init_board(game->board);
}

void init_board(Piece board[BOARD_SIZE][BOARD_SIZE]) {
    // Clear board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j].type = EMPTY;
            board[i][j].color = NONE;
        }
    }

    // Setup black pieces (row 0)
    board[0][0] = (Piece){ROOK, BLACK};
    board[0][1] = (Piece){KNIGHT, BLACK};
    board[0][2] = (Piece){BISHOP, BLACK};
    board[0][3] = (Piece){QUEEN, BLACK};
    board[0][4] = (Piece){KING, BLACK};
    board[0][5] = (Piece){BISHOP, BLACK};
    board[0][6] = (Piece){KNIGHT, BLACK};
    board[0][7] = (Piece){ROOK, BLACK};

    // Black pawns (row 1)
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[1][i] = (Piece){PAWN, BLACK};
    }

    // White pawns (row 6)
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[6][i] = (Piece){PAWN, WHITE};
    }

    // Setup white pieces (row 7)
    board[7][0] = (Piece){ROOK, WHITE};
    board[7][1] = (Piece){KNIGHT, WHITE};
    board[7][2] = (Piece){BISHOP, WHITE};
    board[7][3] = (Piece){QUEEN, WHITE};
    board[7][4] = (Piece){KING, WHITE};
    board[7][5] = (Piece){BISHOP, WHITE};
    board[7][6] = (Piece){KNIGHT, WHITE};
    board[7][7] = (Piece){ROOK, WHITE};
}

int is_valid_position(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

void add_move_if_valid(GameState *game, int row, int col, PieceColor piece_color) {
    if (!is_valid_position(row, col)) return;

    Piece target = game->board[row][col];
    if (target.type == EMPTY || target.color != piece_color) {
        game->possible_moves[game->possible_moves_count].row = row;
        game->possible_moves[game->possible_moves_count].col = col;
        game->possible_moves_count++;
    }
}

void calculate_pawn_moves(GameState *game, Position pos, PieceColor color) {
    int direction = (color == WHITE) ? -1 : 1;
    int start_row = (color == WHITE) ? 6 : 1;

    // Move forward
    if (is_valid_position(pos.row + direction, pos.col) &&
        game->board[pos.row + direction][pos.col].type == EMPTY) {
        add_move_if_valid(game, pos.row + direction, pos.col, color);

        // Double move from start
        if (pos.row == start_row &&
            game->board[pos.row + 2 * direction][pos.col].type == EMPTY) {
            add_move_if_valid(game, pos.row + 2 * direction, pos.col, color);
        }
    }

    // Capture diagonally
    for (int offset = -1; offset <= 1; offset += 2) {
        int new_col = pos.col + offset;
        if (is_valid_position(pos.row + direction, new_col)) {
            Piece target = game->board[pos.row + direction][new_col];
            if (target.type != EMPTY && target.color != color) {
                add_move_if_valid(game, pos.row + direction, new_col, color);
            }
        }
    }
}

void calculate_knight_moves(GameState *game, Position pos, PieceColor color) {
    int knight_moves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    for (int i = 0; i < 8; i++) {
        int new_row = pos.row + knight_moves[i][0];
        int new_col = pos.col + knight_moves[i][1];
        add_move_if_valid(game, new_row, new_col, color);
    }
}

void calculate_sliding_moves(GameState *game, Position pos, PieceColor color,
                             int directions[][2], int num_directions) {
    for (int d = 0; d < num_directions; d++) {
        int dr = directions[d][0];
        int dc = directions[d][1];
        int new_row = pos.row + dr;
        int new_col = pos.col + dc;

        while (is_valid_position(new_row, new_col)) {
            Piece target = game->board[new_row][new_col];

            if (target.type == EMPTY) {
                add_move_if_valid(game, new_row, new_col, color);
            } else {
                if (target.color != color) {
                    add_move_if_valid(game, new_row, new_col, color);
                }
                break;
            }

            new_row += dr;
            new_col += dc;
        }
    }
}

void calculate_possible_moves(GameState *game, Position pos) {
    game->possible_moves_count = 0;

    Piece piece = game->board[pos.row][pos.col];
    if (piece.type == EMPTY) return;

    switch (piece.type) {
        case PAWN:
            calculate_pawn_moves(game, pos, piece.color);
            break;

        case KNIGHT:
            calculate_knight_moves(game, pos, piece.color);
            break;

        case BISHOP: {
            int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
            calculate_sliding_moves(game, pos, piece.color, directions, 4);
            break;
        }

        case ROOK: {
            int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
            calculate_sliding_moves(game, pos, piece.color, directions, 4);
            break;
        }

        case QUEEN: {
            int directions[8][2] = {
                {-1, -1}, {-1, 0}, {-1, 1},
                {0, -1}, {0, 1},
                {1, -1}, {1, 0}, {1, 1}
            };
            calculate_sliding_moves(game, pos, piece.color, directions, 8);
            break;
        }

        case KING: {
            int directions[8][2] = {
                {-1, -1}, {-1, 0}, {-1, 1},
                {0, -1}, {0, 1},
                {1, -1}, {1, 0}, {1, 1}
            };
            for (int i = 0; i < 8; i++) {
                add_move_if_valid(game, pos.row + directions[i][0],
                                pos.col + directions[i][1], piece.color);
            }
            break;
        }

        default:
            break;
    }
}

int is_move_possible(GameState *game, Position to) {
    for (int i = 0; i < game->possible_moves_count; i++) {
        if (game->possible_moves[i].row == to.row &&
            game->possible_moves[i].col == to.col) {
            return 1;
        }
    }
    return 0;
}

int make_move(GameState *game, Position from, Position to) {
    if (!is_move_possible(game, to)) {
        return 0;
    }

    Piece moving_piece = game->board[from.row][from.col];
    Piece captured_piece = game->board[to.row][to.col];

    // Handle capture
    if (captured_piece.type != EMPTY) {
        char symbol = get_piece_symbol(captured_piece);
        if (game->current_turn == WHITE) {
            int len = strlen(game->white_captured);
            game->white_captured[len] = symbol;
            game->white_captured[len + 1] = ' ';
            game->white_captured[len + 2] = '\0';
            game->white_score++;
        } else {
            int len = strlen(game->black_captured);
            game->black_captured[len] = symbol;
            game->black_captured[len + 1] = ' ';
            game->black_captured[len + 2] = '\0';
            game->black_score++;
        }
    }

    // Move piece
    game->board[to.row][to.col] = moving_piece;
    game->board[from.row][from.col] = (Piece){EMPTY, NONE};

    return 1;
}

char get_piece_symbol(Piece piece) {
    switch (piece.type) {
        case PAWN: return 'P';
        case KNIGHT: return 'N';
        case BISHOP: return 'B';
        case ROOK: return 'R';
        case QUEEN: return 'Q';
        case KING: return 'K';
        default: return ' ';
    }
}

const char* get_piece_unicode(Piece piece) {
    if (piece.type == EMPTY) return " ";

    static const char* white_pieces[] = {
        " ", "♙", "♘", "♗", "♖", "♕", "♔"
    };
    static const char* black_pieces[] = {
        " ", "♟", "♞", "♝", "♜", "♛", "♚"
    };

    if (piece.color == WHITE) {
        return white_pieces[piece.type];
    } else {
        return black_pieces[piece.type];
    }
}

const char* time_control_to_string(TimeControl tc) {
    switch (tc) {
        case BULLET: return "Bullet (1+0)";
        case HYPER_BULLET: return "Hyper Bullet (30s)";
        case BLITZ: return "Blitz (3+0)";
        case RAPID: return "Rapid (10+0)";
        default: return "Unknown";
    }
}

void get_player_input(char *buffer, int max_length) {
    if (fgets(buffer, max_length, stdin) != NULL) {
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

void pause_for_user(void) {
    printf("\nPress Enter to continue...");
    getchar();
}
