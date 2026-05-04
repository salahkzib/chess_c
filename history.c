#include "chess.h"

int load_game_history(GameRecord *records) {
    FILE *file = fopen(HISTORY_FILE, "rb");
    if (!file) {
        return 0; // No history file yet
    }

    int count = 0;
    while (fread(&records[count], sizeof(GameRecord), 1, file) == 1) {
        count++;
        if (count >= MAX_GAMES) break;
    }

    fclose(file);
    return count;
}

void save_game_history(GameRecord *records, int count) {
    FILE *file = fopen(HISTORY_FILE, "wb");
    if (!file) {
        printf("Error: Could not save game history!\n");
        return;
    }

    fwrite(records, sizeof(GameRecord), count, file);
    fclose(file);
}

void add_game_to_history(GameState *game) {
    GameRecord records[MAX_GAMES];
    int count = load_game_history(records);

    if (count >= MAX_GAMES) {
        // Shift records to make room (remove oldest)
        for (int i = 0; i < MAX_GAMES - 1; i++) {
            records[i] = records[i + 1];
        }
        count = MAX_GAMES - 1;
    }

    // Create new record
    GameRecord new_record;
    strcpy(new_record.white_player, game->white_player);
    strcpy(new_record.black_player, game->black_player);

    // Determine winner
    if (game->white_score > game->black_score) {
        new_record.winner = WHITE;
    } else if (game->black_score > game->white_score) {
        new_record.winner = BLACK;
    } else {
        new_record.winner = NONE; // Draw
    }

    new_record.time_control = game->time_control;
    new_record.moves = game->move_count;
    new_record.date = time(NULL);

    records[count] = new_record;
    count++;

    save_game_history(records, count);
}
