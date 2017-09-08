#include "SPMainAux.h"

static SPGame* game = NULL;
static int difficulty = -1;

int main(void) {
	char s[SP_MAX_LINE_LENGTH];
	int isRestart = 0;
	do{
		while(difficulty == -1){
			printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
			fgets(s,SP_MAX_LINE_LENGTH,stdin);
			if(s == NULL){
				printf("Error: fgets has failed\n");
				return 0;
			}
			s[strcspn(s, "\r\n")] = 0;
            if (s == "start"){
                SPGame a = spGameCreate();
                spGamePrintBoard(a);
                return 0;
            }
			if(spParserIsInt(s) == true){
				difficulty = atoi(s);
				if(difficulty < 1 || difficulty > 7){
					difficulty = -1;
					printf("Error: invalid level (should be between 1 to 7)\n");
				}
			}
			else{
				SPCommand cmd = spParserPraseLine(s);
				if(cmd.cmd == SP_QUIT){
					printf("Exiting...\n");
					return EXIT_SUCCESS;
				}
				else
					printf("Error: invalid level (should be between 1 to 7)\n");
			}
		}
			if(isRestart){
				game = NULL;
				isRestart = 0;
			}
			if(game == NULL && (game = StartNewGame()) == NULL)
				return 0;
			fgets(s,SP_MAX_LINE_LENGTH,stdin);
			if(s == NULL){
				printf("Error: fgets has failed\n");
				return 0;
			}
			s[strcspn(s, "\r\n")] = 0;
			SPCommand cmd = spParserPraseLine(s);
			game = ExecuteCmd(game , cmd , difficulty);
			if(cmd.cmd == SP_RESTART){
				difficulty = -1;
				printf("Game restarted!\n");
				isRestart = 1;
			}
	}while(game != NULL);
	return 0;
}
