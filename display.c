#include "chess.h"

void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void display_board(GameState *game) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║               ♔ ROYAL CHESS ARENA ♔                           ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");

    // Top border
    printf("    ┌");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("─────");
        if (i < BOARD_SIZE - 1) printf("┬");
    }
    printf("┐\n");

    // Board rows
    for (int row = 0; row < BOARD_SIZE; row++) {
        printf("  %d │", 8 - row);

        for (int col = 0; col < BOARD_SIZE; col++) {
            Piece piece = game->board[row][col];
            int is_selected = game->has_selection &&
                            game->selected.row == row &&
                            game->selected.col == col;
            int is_possible = 0;

            // Check if this is a possible move
            for (int i = 0; i < game->possible_moves_count; i++) {
                if (game->possible_moves[i].row == row &&
                    game->possible_moves[i].col == col) {
                    is_possible = 1;
                    break;
                }
            }

            // Background color
            if (is_selected) {
                printf("\033[43m"); // Yellow background
            } else if (is_possible) {
                printf("\033[42m"); // Green background
            } else if ((row + col) % 2 == 0) {
                printf("\033[47m"); // Light background
            } else {
                printf("\033[100m"); // Dark background
            }

            // Piece color
            if (piece.color == WHITE) {
                printf("\033[97m"); // White foreground
            } else if (piece.color == BLACK) {
                printf("\033[30m"); // Black foreground
            }

            printf(" %s ", get_piece_unicode(piece));
            printf("\033[0m"); // Reset colors

            printf("│");
        }

        printf(" %d", 8 - row);

        // Row separator
        if (row < BOARD_SIZE - 1) {
            printf("\n    ├");
            for (int i = 0; i < BOARD_SIZE; i++) {
                printf("─────");
                if (i < BOARD_SIZE - 1) printf("┼");
            }
            printf("┤\n");
        }
    }

    // Bottom border
    printf("\n    └");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("─────");
        if (i < BOARD_SIZE - 1) printf("┴");
    }
    printf("┘\n");

    // Column labels
    printf("      ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("  %c  ", 'a' + i);
    }
    printf("\n");
}

void display_player_info(GameState *game) {
    printf("\n");
    printf("┌──────────────────────────────────────────────────────────────┐\n");

    // Black player (top)
    printf("│ ⚫ %-20s  Score: %2d  ", game->black_player, game->black_score);
    if (game->current_turn == BLACK) {
        printf("◄ TURN");
    }
    printf("\n");

    printf("│ Captured: %-40s │\n", game->black_captured);

    printf("├──────────────────────────────────────────────────────────────┤\n");
    printf("│ Time Control: %-43s │\n", time_control_to_string(game->time_control));
    printf("│ Move: %3d                                                    │\n",
           (game->move_count / 2) + 1);
    printf("├──────────────────────────────────────────────────────────────┤\n");

    // White player (bottom)
    printf("│ ⚪ %-20s  Score: %2d  ", game->white_player, game->white_score);
    if (game->current_turn == WHITE) {
        printf("◄ TURN");
    }
    printf("\n");

    printf("│ Captured: %-40s │\n", game->white_captured);

    printf("└──────────────────────────────────────────────────────────────┘\n");
}

void display_history(GameRecord *records, int count) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                     🏆 GAME HISTORY 🏆                         ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");

    if (count == 0) {
        printf("  No games played yet.\n");
        printf("  Start a game to build your history!\n");
        return;
    }

    for (int i = count - 1; i >= 0; i--) {
        GameRecord *game = &records[i];
        char time_str[26];
        struct tm *tm_info = localtime(&game->date);
        strftime(time_str, 26, "%Y-%m-%d %H:%M", tm_info);

        printf("┌──────────────────────────────────────────────────────────────┐\n");
        printf("│ Game #%d - %s                                    │\n",
               count - i, time_str);
        printf("├──────────────────────────────────────────────────────────────┤\n");

        printf("│ ⚪ %-25s ", game->white_player);
        if (game->winner == WHITE) printf("🏆 WINNER");
        printf("\n");

        printf("│ ⚫ %-25s ", game->black_player);
        if (game->winner == BLACK) printf("🏆 WINNER");
        printf("\n");

        if (game->winner == NONE) {
            printf("│ Result: DRAW                                                 │\n");
        }

        printf("│ Time Control: %-25s  Moves: %3d              │\n",
               time_control_to_string(game->time_control), game->moves);
        printf("└──────────────────────────────────────────────────────────────┘\n\n");
    }
}
