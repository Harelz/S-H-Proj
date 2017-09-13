#include <stdio.h>
#include "SPParser.h"
#define CHECK_VALID(spcmd) if(strtok(NULL , " \t\r\n") == NULL) spcmd.validArg = true

bool spParserIsInt(const char* str){
	if(str == NULL) return false;
	int len = strlen(str);
	if(len == 0) return false;
	int i = 0;
	if(str[0]=='-'){
		if(len==1)
			return false;
		i++;
	}
	len--;
	for( int j = i; j < len; j++)
		if(!(str[j]>='0' && str[j]<='9'))
			return false;
	return true;
}

bool spParserIsAtoH(const char* str){
	if(str == NULL || strlen(str) != 1) return false;
	if (str[0] > 64 && str[0] < 91)
		return true;
	else
		return false;
}

char** spTilesParse(const char* str);

SPGameCommand spGameParser(const char* str){
	char s[SP_MAX_LINE_LENGTH];
	strcpy(s,str);
	SPGameCommand spcmd;
	if (s == NULL){
		spcmd.cmd = SP_GINVALID_LINE;
		return spcmd;
	}
	char* token = (char*)strtok(s, " \t\r\n");
	if(token == NULL){
		spcmd.cmd = SP_GINVALID_LINE;
		return spcmd;
	}
	else if (strcmp(token,"move") == 0)
		spcmd.cmd = SP_MOVE;
	else if (strcmp(token,"get_moves") == 0)
		spcmd.cmd = SP_GET_MOVES;
	else if (strcmp(token,"save") == 0)
		spcmd.cmd = SP_SAVE;
	else if (strcmp(token,"undo") == 0)
		spcmd.cmd = SP_UNDO;
	else if (strcmp(token,"reset") == 0)
		spcmd.cmd = SP_RESET;
	else if (strcmp(token,"quit") == 0)
		spcmd.cmd = SP_GQUIT;
	else
		spcmd.cmd = SP_GINVALID_LINE;
	token = (char*)strtok(NULL , " \t\r\n");
	if(token == NULL) {
		spcmd.arg = -1;
		return spcmd;
	}
	else if(spcmd.cmd == SP_SAVE) {
		spcmd.pathArg = token;
		CHECK_VALID(spcmd);
	}
	else if(spcmd.cmd == SP_START || spcmd.cmd == SP_GQUIT || spcmd.cmd == SP_PRINT ||spcmd.cmd == SP_DEFAULT)
		spcmd.cmd = SP_GINVALID_LINE;
	else if(spParserIsInt(token)){
		spcmd.arg = atoi(token);
		CHECK_VALID(spcmd);
	}
	return spcmd;
}

SPSettingCommand spSettingsParser(const char* str){
	char s[SP_MAX_LINE_LENGTH];
	strcpy(s,str);
	SPSettingCommand spcmd;
	if (s == NULL){
		spcmd.cmd = SP_SINVALID_LINE;
		return spcmd;
	}
	char* token = (char*)strtok(s, " \t\r\n");
	if(token == NULL){
		spcmd.cmd = SP_SINVALID_LINE;
		return spcmd;
	}
	else if (strcmp(token,"game_mode") == 0)
		spcmd.cmd = SP_CHOOSE_GAME_MODE;
    else if (strcmp(token,"difficulty") == 0)
        spcmd.cmd = SP_DIFFICULTY;
	else if (strcmp(token,"user_color") == 0)
		spcmd.cmd = SP_CHOOSE_USER_COLOR;
    else if (strcmp(token,"load") == 0)
        spcmd.cmd = SP_LOAD;
    else if (strcmp(token,"default") == 0)
        spcmd.cmd = SP_DEFAULT;
    else if (strcmp(token,"print_setting") == 0)
        spcmd.cmd = SP_PRINT;
	else if (strcmp(token,"quit") == 0)
		spcmd.cmd = SP_SQUIT;
	else if (strcmp(token,"start") == 0)
		spcmd.cmd = SP_START;
	else
		spcmd.cmd = SP_SINVALID_LINE;
	token = (char*)strtok(NULL , " \t\r\n");
	if(token == NULL) {
        spcmd.arg = -1;
        return spcmd;
    }
    else if(spcmd.cmd == SP_LOAD) {
        spcmd.pathArg = token;
        CHECK_VALID(spcmd);
    }
	else if(spcmd.cmd == SP_START || spcmd.cmd == SP_SQUIT || spcmd.cmd == SP_PRINT ||spcmd.cmd == SP_DEFAULT)
		spcmd.cmd = SP_SINVALID_LINE;
    else if(spParserIsInt(token)){
		spcmd.arg = atoi(token);
        CHECK_VALID(spcmd);
    }
	return spcmd;
}

