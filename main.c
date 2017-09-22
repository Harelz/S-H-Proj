#include "SPGame.h"
#include "SPParser.h"


int main() {
	char s[SP_MAX_LINE_LENGTH];
    SPGame *game;
    int spStatus = 0;
	int isRestart = 0;
	do{ // mode : 0 - settings, 1 - game, 2 - load, -1 - exit.
        game = spGameCreateDef();
		while(spStatus == 0){
            printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
			fgets(s,SP_MAX_LINE_LENGTH,stdin);
			if(s == NULL){
				printf("Error: fgets has failed\n");
				return 0;
			}
			s[strcspn(s, "\r\n")] = 0;
            SPSettingCommand cmd = spSettingsParser(s);
            spStatus = settingsHandler(game->settings, cmd);
            if (spStatus == 2)
                spStatus = loadGame(game, cmd.pathArg);
		}
        while (spStatus == 1) {
            spGamePrintBoard(game);
            printf("Wawawa\n");
            fgets(s, SP_MAX_LINE_LENGTH, stdin);
            if(s == NULL){
                printf("Error: fgets has failed\n");
                return 0;
            }
            s[strcspn(s, "\r\n")] = 0;
            SPGameCommand cmd = spGameParser(s);
            spStatus = spGameHandler(game, cmd);
        }
	}while(spStatus != -1);
    spGameDestroy(game);
    return EXIT_SUCCESS;
}