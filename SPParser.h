#ifndef SPPARSER_H_
#define SPPARSER_H_
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "SPMoves.h"
#include <ctype.h>

//specify the maximum line length
#define SP_MAX_LINE_LENGTH 1024

#define IN_RANGE(i, min, max) ((i >= min) && (i < max))
#define PRINT_INVALID_COMMAND printf("Error: invalid command\n")
#define IS_VALID(cmd) if(!cmd.validArg) PRINT_INVALID_COMMAND

//a type used to represent a command
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

//a new type that is used to encapsulate a parsed line
typedef struct {
	SP_SETTING_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	char* pathArg;
	int arg;
} SPSettingCommand;

typedef enum {
	SP_MOVE,
	SP_GET_MOVES,
	SP_SAVE,
	SP_UNDO,
	SP_RESET,
	SP_GQUIT,
	SP_GINVALID_LINE,
	SP_CASTLE
} SP_GAME_COMMAND;

/*
typedef struct{
	int row;
	int column;
} SPTile;

typedef struct {
	SPTile from;
	SPTile to;
} SPMove;*/

//a new type that is used to encapsulate a parsed line
typedef struct  {
	SP_GAME_COMMAND cmd;
	bool validArg; //is set to true if the line contains a valid argument
	char* pathArg;
	SPMove* move;
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
 * Parses a specified line. If the line is a command which has an integer
 * argument then the argument is parsed and is saved in the field arg and the
 * field validArg is set to true. In any other case then 'validArg' is set to
 * false and the value 'arg' is undefined
 *
 * @return
 * A parsed line such that:
 *   cmd - contains the command type, if the line is invalid then this field is
 *         set to INVALID_LINE
 *   validArg - is set to true if the command is add_disc and the integer argument
 *              is valid
 *   arg      - the integer argument in case validArg is set to true
 */

bool spParserIsAtoH(const char* str);

SPSettingCommand spSettingsParser(const char* str);

SPGameCommand spGameParser(const char* str);

SPTile* spParseTile(char* s);

#endif
