#include "SPParser.h"

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

SPCommand spParserPraseLine(const char* str){
	char s[SP_MAX_LINE_LENGTH];
	strcpy(s,str);
	SPCommand spcmd;
	spcmd.validArg = false;
	if (s == NULL){
		spcmd.cmd = SP_INVALID_LINE;
		return spcmd;
	}
	char* token = (char*)strtok(s, " \t\r\n");
	if(token == NULL){
		spcmd.cmd = SP_INVALID_LINE;
		return spcmd;
	}
	else if (strcmp(token,"suggest_move") == 0)
		spcmd.cmd = SP_SUGGEST_MOVE;
	else if (strcmp(token,"undo_move") == 0)
		spcmd.cmd = SP_UNDO_MOVE;
	else if (strcmp(token,"quit") == 0)
		spcmd.cmd = SP_QUIT;
	else if (strcmp(token,"restart_game") == 0)
		spcmd.cmd = SP_RESTART;
	else if (strcmp(token,"add_disc") == 0)
		spcmd.cmd = SP_ADD_DISC;
	else
		spcmd.cmd = SP_INVALID_LINE;
	token = (char*)strtok(NULL , " \t\r\n");
	if(token == NULL)
		return spcmd;
	else if(spcmd.cmd != SP_ADD_DISC)
		spcmd.cmd = SP_INVALID_LINE;
	else if(spParserIsInt(token)){
		spcmd.arg = atoi(token);
		if(strtok(NULL , " \t\r\n") == NULL)
			spcmd.validArg = true;
	}
	return spcmd;
}

