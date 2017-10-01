#ifndef SPSETTINGS_H_
#define SPSETTINGS_H_

#include <stdio.h>
#include "SPParser.h"

// enum which presents the game modes available
typedef enum sp_game_mode_t {
	SP_MODE_1P = 1,
	SP_MODE_2P = 2
} SP_GAME_MODE;

// enum which presents the difficulties available
typedef enum sp_difficulty_t {
	SP_DIFF_NOOB = 1,
	SP_DIFF_EASY = 2,
	SP_DIFF_MODERATE = 3,
	SP_DIFF_HARD = 4
//	,SP_DIFF_EXPERT
} SP_GAME_DIFFICULTY;

// enum which presents the colors available for the user
typedef enum sp_user_color_t {
	SP_USER_COLOR_BLACK = 0,
	SP_USER_COLOR_WHITE = 1
} SP_USER_COLOR;

/* the Settings struct, which contains values of : game-mode , game-difficulty, current player's turn and
 * the color of player number 1
 */
typedef struct sp_game_sett_t {
    SP_USER_COLOR curr_turn;
	SP_GAME_MODE game_mode;
	SP_GAME_DIFFICULTY difficulty;
	SP_USER_COLOR p1_color;
} SPSettings;

/*
 * The function modifies the settings given according to the command
 */
int settingsHandler(SPSettings* settings, SPSettingCommand cmd);

/**
 * Creates a new settings with the parameters given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new settings instant is returned.
 */
SPSettings* init_settings(SP_GAME_MODE mode, SP_GAME_DIFFICULTY diff, SP_USER_COLOR color);



/**
 * Creates a new settings with default values, and destroys the settings given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new settings instant is returned.
 */
SPSettings* defaultSettings(SPSettings* settings);

/**
 * Sets game mode value of the settings given to the mode given
 */
void set_game_mode(SPSettings* settings, SP_GAME_MODE mode);

/**
 * Sets difficulty value of the settings given to the mode given
 */
void set_difficulty(SPSettings* settings, SP_GAME_DIFFICULTY difficulty);

/**
 * Sets user color value of the settings given to the mode given
 */
void set_user_color(SPSettings* settings, SP_USER_COLOR color);

/**
 * Prints Settings' values in the console
 */
void settings_print(SPSettings* settings);

/**
 * free the memory allocated by the settings given
 */
void spSettingsDestroy(SPSettings* settings);


/**
 * Creates a new settings with values identical to the settings given
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a copied settings instant is returned.
 */

SPSettings* spSettingsCopy(SPSettings* src);

#endif
