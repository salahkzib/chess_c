#include "chess.h"

void display_main_menu(void) {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║      ♔ ROYAL CHESS ARENA ♔            ║\n");
    printf("║    Master the game of kings            ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n");
    printf("1. Start New Game\n");
    printf("2. View Game History\n");
    printf("3. Change Player Names\n");
    printf("4. Select Time Control\n");
    printf("5. Exit\n");
    printf("\nChoice: ");
}

void change_player_names(GameState *game) {
    clear_screen();
    printf("═══ Change Player Names ═══\n\n");

    printf("White Player (current: %s): ", game->white_player);
    char temp[MAX_NAME_LENGTH];
    get_player_input(temp, MAX_NAME_LENGTH);
    if (strlen(temp) > 0) {
        strcpy(game->white_player, temp);
    }

    printf("Black Player (current: %s): ", game->black_player);
    get_player_input(temp, MAX_NAME_LENGTH);
    if (strlen(temp) > 0) {
        strcpy(game->black_player, temp);
    }

    printf("\nPlayer names updated!\n");
    pause_for_user();
}

void select_time_control(GameState *game) {
    clear_screen();
    printf("═══ Select Time Control ═══\n\n");
    printf("1. Bullet (1+0)\n");
    printf("2. Hyper Bullet (30s)\n");
    printf("3. Blitz (3+0)\n");
    printf("4. Rapid (10+0)\n");
    printf("\nChoice: ");

    int choice;
    scanf("%d", &choice);
    getchar(); // consume newline

    switch(choice) {
        case 1: game->time_control = BULLET; break;
        case 2: game->time_control = HYPER_BULLET; break;
        case 3: game->time_control = BLITZ; break;
        case 4: game->time_control = RAPID; break;
        default:
            printf("Invalid choice. Keeping current selection.\n");
            pause_for_user();
            return;
    }

    printf("\nTime control set to: %s\n", time_control_to_string(game->time_control));
    pause_for_user();
}

void play_game(GameState *game) {
    clear_screen();
    game->game_started = 1;
    game->current_turn = WHITE;
    game->move_count = 0;
    game->white_score = 0;
    game->black_score = 0;
    game->has_selection = 0;
    strcpy(game->white_captured, "");
    strcpy(game->black_captured, "");
    init_board(game->board);

    while (game->game_started) {
        clear_screen();
        display_board(game);
        display_player_info(game);

        printf("\n");
        if (game->has_selection) {
            printf("Selected: %c%d\n", 'a' + game->selected.col, 8 - game->selected.row);
            printf("Enter destination (e.g., e4) or 'x' to cancel: ");
        } else {
            printf("Enter piece to move (e.g., e2) or 'q' to quit: ");
        }

        char input[10];
        get_player_input(input, 10);

        if (input[0] == 'q' || input[0] == 'Q') {
            printf("\nSave this game to history? (y/n): ");
            char save;
            scanf("%c", &save);
            getchar();

            if (save == 'y' || save == 'Y') {
                add_game_to_history(game);
                printf("Game saved!\n");
            }

            game->game_started = 0;
            pause_for_user();
            break;
        }

        if (input[0] == 'x' || input[0] == 'X') {
            game->has_selection = 0;
            continue;
        }

        // Parse position
        if (strlen(input) < 2) {
            printf("Invalid input! Use format like 'e2'\n");
            pause_for_user();
            continue;
        }

        int col = input[0] - 'a';
        int row = 8 - (input[1] - '0');

        if (!is_valid_position(row, col)) {
            printf("Invalid position!\n");
            pause_for_user();
            continue;
        }

        Position pos = {row, col};

        if (!game->has_selection) {
            // Select piece
            Piece piece = game->board[row][col];
            if (piece.type == EMPTY) {
                printf("No piece at that position!\n");
                pause_for_user();
                continue;
            }

            if (piece.color != game->current_turn) {
                printf("That's not your piece!\n");
                pause_for_user();
                continue;
            }

            game->selected = pos;
            game->has_selection = 1;
            calculate_possible_moves(game, pos);
        } else {
            // Make move
            if (make_move(game, game->selected, pos)) {
                game->has_selection = 0;
                game->current_turn = (game->current_turn == WHITE) ? BLACK : WHITE;
                game->move_count++;
            } else {
                printf("Invalid move!\n");
                pause_for_user();
            }
        }
    }
}

int main(void) {
    GameState game;
    init_game(&game);

    int running = 1;
    while (running) {
        clear_screen();
        display_main_menu();

        int choice;
        scanf("%d", &choice);
        getchar(); // consume newline

        switch(choice) {
            case 1:
                play_game(&game);
                break;

            case 2: {
                clear_screen();
                GameRecord records[MAX_GAMES];
                int count = load_game_history(records);
                display_history(records, count);
                pause_for_user();
                break;
            }

            case 3:
                change_player_names(&game);
                break;

            case 4:
                select_time_control(&game);
                break;

            case 5:
                printf("\nThanks for playing! Goodbye.\n");
                running = 0;
                break;

            default:
                printf("Invalid choice!\n");
                pause_for_user();
        }
    }

    return 0;
}
