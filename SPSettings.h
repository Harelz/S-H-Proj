#ifndef SPSETTINGS_H_
#define SPSETTINGS_H_

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum sp_game_mode_t {
	SP_MODE_1P = 1,
	SP_MODE_2P
} SP_GAME_MODE;

typedef enum sp_difficulty_t {
	SP_DIFF_NOOB = 1,
	SP_DIFF_EASY,
	SP_DIFF_MODERATE,
	SP_DIFF_HARD
//	,SP_DIFF_EXPERT
} SP_GAME_DIFFICULTY;

typedef enum sp_user_color_t {
	SP_USER_COLOR_WHITE = 1,
	SP_USER_COLOR_BLACK
} SP_USER_COLOR;

typedef struct sp_game_sett_t {
	SP_GAME_MODE game_mode;
	SP_GAME_DIFFICULTY difficulty;
	SP_USER_COLOR color;
} game_settings;

void setting_handler();

game_settings* init_settings(SP_GAME_MODE mode, SP_GAME_DIFFICULTY diff, SP_USER_COLOR color);

game_settings* load(char* fpath);

game_settings* default_values();

void set_game_mode(game_settings* settings, SP_GAME_MODE mode);

void set_difficulty(game_settings* settings, SP_GAME_DIFFICULTY difficulty);

void set_user_color(game_sett* settings, SP_USER_COLOR color);

void print(game_settings* settings);

void start(game_settings*);

#endif
