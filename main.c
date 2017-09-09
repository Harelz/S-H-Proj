#include "SPMainAux.h"

static SPGame* game = NULL;
static int mode = 0;

int main(void) {
	char s[SP_MAX_LINE_LENGTH];
    game = spGameCreate();
	int isRestart = 0;
	do{
		while(mode == 0){
            printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
			fgets(s,SP_MAX_LINE_LENGTH,stdin);
			if(s == NULL){
				printf("Error: fgets has failed\n");
				return 0;
			}
			s[strcspn(s, "\r\n")] = 0;
            SPCommand cmd = spSettingsParser(s);
            mode = settingsHandler(game->settings, cmd);
            if(mode == 2){
                spGameDestroy(game);
                return EXIT_SUCCESS;
            }
		}
			if (s == "start" || s[5] == '\0'){
				SPGame* a = spGameCreate();
				spGamePrintBoard(a);
				return 0;
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
			SPCommand cmd = spSettingsParser(s);
			game = ExecuteCmd(game , cmd , mode);
			if(cmd.cmd == SP_START){
				mode = -1;
				printf("Game restarted!\n");
				isRestart = 1;
			}
	}while(game != NULL);
	return 0;
}
