#include "SPGame.h"
#include "SPParser.h"

static SPGame* game = NULL;
static int mode = 0;

int main(void) {
	char s[SP_MAX_LINE_LENGTH];
    SPGame *game;
	int isRestart = 0;
	do{ // mode : 0 - settings, 1 - game, 2 - load, -1 - exit.
        game = spGameCreateDef();
		while(mode == 0){
            printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
			fgets(s,SP_MAX_LINE_LENGTH,stdin);
			if(s == NULL){
				printf("Error: fgets has failed\n");
				return 0;
			}
			s[strcspn(s, "\r\n")] = 0;
            SPSettingCommand cmd = spSettingsParser(s);
            mode = settingsHandler(game->settings, cmd);
            if (mode == 2)
                mode = loadGame(game, cmd.pathArg);
		}
        while (mode == 1) {
            spGamePrintBoard(game);
            printf("Wawawa\n");
            fgets(s, SP_MAX_LINE_LENGTH, stdin);
            if(s == NULL){
                printf("Error: fgets has failed\n");
                return 0;
            }
            s[strcspn(s, "\r\n")] = 0;
            SPGameCommand cmd = spGameParser(s);
            mode = gameHandler(game, cmd);
        }
	}while(mode != -1);
    spGameDestroy(game);
    return EXIT_SUCCESS;
}