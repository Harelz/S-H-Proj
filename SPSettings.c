#include "SPSettings.h"

game_settings* init_settings(SP_GAME_MODE mode, SP_GAME_DIFFICULTY diff, SP_USER_COLOR color) {
	game_settings* settings = (game_settings*)calloc(1, sizeof(game_settings));
	set_game_mode(settings, mode);
	set_difficulty(settings, diff);
	set_user_color(settings, color);
	return settings;
}

void setting_handler() {
}

void set_game_mode(game_settings* settings, SP_GAME_MODE mode) {
		settings -> game_mode = mode;
}

void set_difficulty(game_settings* settings, SP_GAME_DIFFICULTY difficulty)  {
	settings -> difficulty = difficulty;
}

void set_user_color(game_settings* settings, SP_USER_COLOR color) {
	settings -> color = color;
}


game_settings* load(char* fpath) {
	char * data = get_data_from_file(fpath);

}

game_sett* default_values() {
	return init_settings(1, 2, 1);
}

void print(game_settings* settings);

void start(game_settings*);

char* get_data_from_file(char* fpath) {
	char c = '';
	char* data = NULL;
	FILE* f = fopen(fpath);

}
