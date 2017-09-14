//
// Created by hoshri on 9/11/2017.
//

#include "SPGameFlow.h"

/*

int gameHandler(SPGame* game, SPGameCommand cmd) {
    switch(cmd.cmd){
        case SP_RESET:
            printf("Restarting...\n");
            spGameDestroy(game);
            return 1; //indicates to run main all over again
        case SP_GQUIT:
            printf("Exiting...\n");
            return 2; //indicates to terminate
        case SP_UNDO:
            memcpy(game -> gameBoard, spQueuePop(game->history) -> data, sizeof(game->gameBoard));
            if(game->settings->game_mode == SP_MODE_2P){
                printf("Error: set user color command is only vaild for 1-player mode\n");
                return 0;
            }
            else if (IN_RANGE(cmd.arg, -1,2))
                set_user_color(0, cmd.arg);
            else if (cmd.arg == -1)
                set_game_mode(0, 1);
            else
                PRINT_INVALID_COMMAND;
            return 0;
        case SP_SAVE:
            IS_VALID(cmd);
            saveGame(game, cmd.pathArg);
            return 0;
        case SP_MOVE:
            IS_VALID(cmd);
            if (IN_RANGE(cmd.arg, 0,3))
                set_game_mode(0, cmd.arg);
            else if (cmd.arg == -1)
                set_game_mode(0, 1);
            else
                printf("Wrong game mode\n");
            return 0;
        case SP_GET_MOVES:
            IS_VALID(cmd);
            if (IN_RANGE(cmd.arg, 0,5))
                set_difficulty(0, cmd.arg);
            else if (cmd.arg == 5)
                printf("Expert level not supported, please choose a value between 1 to 4:\n");
            else if (cmd.arg == -1)
                set_difficulty(0, 2);
            else
                printf("Wrong difficulty level. The value should be between 1 to 5\n");
            return 0;
        case SP_GINVALID_LINE:
            PRINT_INVALID_COMMAND;
            return 0;
    }
    return 0;
}

int saveGame(SPGame* game, char* fpath){
    FILE *fp;
    int i,j;
    char tile;
    fp = fopen(fpath, "w+");
    if (fp == NULL) {
        printf("File cannot be created or modified\n");
        return 0;
    }
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<game>\n");
    fprintf(fp, "\t<current_turn>%d</current_turn>\n", game->settings->p1_color);
    fprintf(fp, "\t<game_mode>%d</game_mode>\n", game->settings->game_mode);
    if (game->settings->game_mode == 1) {
        fprintf(fp, "\t<difficulty>%d</difficulty>\n", game->settings->difficulty);
        fprintf(fp, "\t<current_turn>%d</current_turn>\n", game->settings->p1_color);
    }
    fprintf(fp, "\t<board>\n");
    for (i = 8; i > 0; i--){
        fprintf(fp, "\t\t<row_%d>",i);
        for (j = 0; j < 8; j++){
            if ((tile = game->gameBoard[i-1][j]) == SP_GAME_EMPTY_ENTRY)
                fprintf(fp, "_");
            else
                fprintf(fp, "%c", tile);
        }
        fprintf(fp, "</row_%d>\n",i);
    }
    fprintf(fp, "\t</board>\n");
    fprintf(fp, "</game>\n");
    fclose(fp);
    return 1;
}*/
