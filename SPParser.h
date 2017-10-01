#ifndef SPPARSER_H_
#define SPPARSER_H_

#include <string.h>
#include "SPMoves.h"
#include <ctype.h>

//specify the maximum line length
#define SP_MAX_LINE_LENGTH 1024

#define IN_RANGE(i, min, max) (((i) >= (min)) && ((i) < (max)))
#define PRINT_INVALID_COMMAND printf("Error: invalid command\n")
#define IS_VALID(cmd) if(!(cmd).validArg) PRINT_INVALID_COMMAND

//a enum used to represent a command
typedef enum {
	SP_CHOOSE_GAME_MODE,
	SP_DIFFICULTY,
	SP_CHOOSE_USER_COLOR,
	SP_LOAD,
	SP_DEFAULT,
	SP_PRINT,
	SP_SQUIT,
	SP_START,
	SP_SINVALID_LINE,
} SP_SETTING_COMMAND;

//a struct that is used to encapsulate a parsed settings' command line
typedef struct {
	SP_SETTING_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	char* pathArg;
	int arg;
} SPSettingCommand;

// enum which represents types of command lines
typedef enum {
	SP_MOVE,
	SP_GET_MOVES,
	SP_SAVE,
	SP_UNDO,
	SP_RESET,
	SP_GQUIT,
	SP_GINVALID_LINE
} SP_GAME_COMMAND;

//a struct that is used to encapsulate a parsed game's command line
typedef struct  {
	SP_GAME_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	char* pathArg;
	SPMove* move;
	SPTile* tile;
	int arg;
} SPGameCommand;

/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool spParserIsInt(const char* str);

/**
 * Parses a specified line. If the line is a valid settings' command then
 * the the information about the command is saved in the SettingsCommand struct
 * and the field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 *   an SPSettingCommand which holds the information of the parsed string given
 */
SPSettingCommand spSettingsParser(const char* str);


/**
 * Parses a specified line. If the line is a valid games' command then
 * the the information about the command is saved in the GameCommand struct
 * and the field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
*   an SPGameCommand which holds the information of the parsed string given
 */
SPGameCommand spGameParser(const char* str);

/**
 * Parses a specified string. If the string represents a tile then the argument
 * is parsed and returned as a tile.
 *
 * @return
 * NULL if the string doesnt represent a tile.
 * Otherwise, on success, returns the tile represented by the string.
 */
SPTile* spParseTile(char* s);

#endif
