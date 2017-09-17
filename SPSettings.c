#include "SPSettings.h"

SPSettings* init_settings(SP_GAME_MODE mode, SP_GAME_DIFFICULTY diff, SP_USER_COLOR color) {
    SPSettings* settings = (SPSettings*)calloc(1, sizeof(SPSettings));
    if(settings == NULL) return NULL;
	set_game_mode(settings, mode);
	set_difficulty(settings, diff);
	set_user_color(settings, color);
	return settings;
}

int settingsHandler(SPSettings* settings, SPSettingCommand cmd) {
    switch(cmd.cmd){
        case SP_START:
            return 1;
        case SP_SQUIT:
            printf("Exiting...\n");
            return -1;
        case SP_CHOOSE_GAME_MODE:
            IS_VALID(cmd);
            if (IN_RANGE(cmd.arg, 1,3))
                set_game_mode(settings, cmd.arg);
            else if (cmd.arg == -1)
                set_game_mode(settings, 1);
            else
                printf("Wrong game mode\n");
            return 0;
        case SP_DIFFICULTY:
            IS_VALID(cmd);
            if (IN_RANGE(cmd.arg, 1,5))
                set_difficulty(settings, cmd.arg);
            else if (cmd.arg == 5)
                printf("Expert level not supported, please choose a value between 1 to 4:\n");
            else if (cmd.arg == -1)
                set_difficulty(settings, 2);
            else
                printf("Wrong difficulty level. The value should be between 1 to 5\n");
            return 0;
        case SP_CHOOSE_USER_COLOR:
            IS_VALID(cmd);
            if(settings->game_mode == SP_MODE_2P){
                printf("Error: set user p1_color command is only vaild for 1-player mode\n");
                return 0;
            }
            else if (IN_RANGE(cmd.arg, 0,2))
                set_user_color(settings, cmd.arg);
            else if (cmd.arg == -1)
                set_game_mode(settings, SP_MODE_1P);
            else
                PRINT_INVALID_COMMAND;
            return 0;
        case SP_LOAD:
            IS_VALID(cmd);
            return 2;
        case SP_DEFAULT:
            defaultSettings(settings);
            return 0;
        case SP_PRINT:
            settings_print(settings);
            return 0;
        case SP_SINVALID_LINE:
            PRINT_INVALID_COMMAND;
            return 0;
    }
    return 0;
}

void set_game_mode(SPSettings* settings, SP_GAME_MODE mode) {
    settings -> game_mode = mode;
    if (mode == SP_MODE_1P)
        printf("Game mode is set to 1 player\n");
    else
        printf("Game mode is set to 2 players\n");
}

void set_difficulty(SPSettings* settings, SP_GAME_DIFFICULTY difficulty)  {
	settings -> difficulty = difficulty;
}

void set_user_color(SPSettings* settings, SP_USER_COLOR color) {
	settings -> p1_color = color;
}


SPSettings* defaultSettings(SPSettings* settings) {
    spSettingsDestroy(settings);
	return init_settings(SP_MODE_1P, SP_DIFF_EASY, SP_USER_COLOR_WHITE);
}

void settings_print(SPSettings* settings){
    printf("SETTINGS:\nGAME_MODE: ");
    if (settings->game_mode == SP_MODE_1P)
        printf("1\n");
    else{
        printf("2\nDIFFICULTY_LVL: %d\n", settings->difficulty);
        printf("USER_CLR: ");
        if (settings->p1_color == SP_USER_COLOR_WHITE)
            printf("WHITE\n");
        else
            printf("BLACK\n");
    }
}

SPSettings* spSettingsCopy(SPSettings* src){
    SPSettings* settings = (SPSettings*)calloc(1, sizeof(SPSettings));
    if(settings == NULL) return NULL;
    settings->game_mode = src->game_mode;
    settings->difficulty = src->difficulty;
    settings->p1_color = src->p1_color;
    return settings;
}

void spSettingsDestroy(SPSettings* settings){
    if(settings != NULL){
        free(settings);
    }
}