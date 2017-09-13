#include "SPMainAux.h"

static SPGame* game = NULL;
static int mode = 0;

int main(void) {
	char s[SP_MAX_LINE_LENGTH];
    game = spGameCreateDef();
	try_save(game);
	exit(1);
    /*
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
            SPSettingCommand cmd = spSettingsParser(s);
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
			SPSettingCommand cmd = spSettingsParser(s);
			game = ExecuteCmd(game , cmd , mode);
			if(cmd.cmd == SP_START){
				mode = -1;
				printf("Game restarted!\n");
				isRestart = 1;
			}
	}while(game != NULL);
	return 0;*/
}

int try_save(SPGame* game){
	FILE *fp;
    int i,j;
    char tile;
	fp = fopen("test.xml", "w+");
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<game>\n");
    fprintf(fp, "\t<current_turn>%d</current_turn>\n", game->settings->p1_color);
    fprintf(fp, "\t<game_mode>%d</game_mode>\n", game->settings->game_mode);
    if (game->settings->game_mode == 1) {
        fprintf(fp, "\t<difficulty>%d</difficulty>\n", game->settings->difficulty);
        fprintf(fp, "\t<current_turn>%d</current_turn>\n", game->settings->p1_color);
    }
    fprintf(fp, "\t<board>\n");
    for (i = 8; i > 0; i--){
        fprintf(fp, "\t\t<row_%d>",i);
        for (j = 0; j < 8; j++){
            if ((tile = game->gameBoard[i-1][j]) == SP_GAME_EMPTY_ENTRY)
                fprintf(fp, "_");
            else
                fprintf(fp, "%c", tile);
        }
        fprintf(fp, "</row_%d>\n",i);
    }
    fprintf(fp, "\t</board>\n");
    fprintf(fp, "</game>\n");
	fclose(fp);
}