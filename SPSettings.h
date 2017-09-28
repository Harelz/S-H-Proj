#ifndef SPSETTINGS_H_
#define SPSETTINGS_H_

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPParser.h" // include SPGame.h which includes SPTileArrayList

typedef enum sp_game_mode_t {
	SP_MODE_1P = 1,
	SP_MODE_2P = 2
} SP_GAME_MODE;

typedef enum sp_difficulty_t {
	SP_DIFF_NOOB = 1,
	SP_DIFF_EASY = 2,
	SP_DIFF_MODERATE = 3,
	SP_DIFF_HARD = 4
//	,SP_DIFF_EXPERT
} SP_GAME_DIFFICULTY;

typedef enum sp_user_color_t {
	SP_USER_COLOR_BLACK = 0,
	SP_USER_COLOR_WHITE = 1
} SP_USER_COLOR;

typedef struct sp_game_sett_t {
    SP_USER_COLOR curr_turn;
	SP_GAME_MODE game_mode;
	SP_GAME_DIFFICULTY difficulty;
	SP_USER_COLOR p1_color;
} SPSettings;

int settingsHandler(SPSettings* settings, SPSettingCommand cmd);

SPSettings* init_settings(SP_GAME_MODE mode, SP_GAME_DIFFICULTY diff, SP_USER_COLOR color);

SPSettings* defaultSettings(SPSettings* settings);

void set_game_mode(SPSettings* settings, SP_GAME_MODE mode);

void set_difficulty(SPSettings* settings, SP_GAME_DIFFICULTY difficulty);

void set_user_color(SPSettings* settings, SP_USER_COLOR color);

void settings_print(SPSettings* settings);

void spSettingsDestroy(SPSettings* settings);

SPSettings* spSettingsCopy(SPSettings* src);

#endif
