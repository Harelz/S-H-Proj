#include "SPGame.h"
#include "GUI/SPGUIManager.h"

int main(int argc, char* argv[]) {
    if(argc == 2 && strcmp(argv[1], "-g") == 0) { //GUI MODE
        isConsole = false;
        return MainGUIManager();
    }
    else if(argc == 1 || (argc == 2 && strcmp(argv[1], "-c") == 0)) { //DEFAULT MODE - CONSOLE
        isConsole = true;
        char s[SP_MAX_LINE_LENGTH];
        SPGame *game;
        int spStatus = 0;
        do { // mode : 0 - settings, 1 - game, 2 - load, -1 - exit, 3 - game without print. 4 - sett without print
            game = spGameCreateDef();
            while (spStatus == 0 || spStatus == 4) {
                if (spStatus != 4)
                    printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
                fgets(s, SP_MAX_LINE_LENGTH, stdin);
                if (s == NULL) {
                    printf("Error: fgets has failed\n");
                    return 0;
                }
                s[strcspn(s, "\r\n")] = 0;
                SPSettingCommand cmd = spSettingsParser(s);
                spStatus = settingsHandler(game->settings, cmd);
                if (spStatus == 2)
                    spStatus = loadGame(game, cmd.pathArg);
            }
            while (spStatus == 1 || spStatus == 3) {
                if (game->settings->game_mode == SP_MODE_1P && game->settings->curr_turn != game->settings->p1_color) {
                    spSetNaiveCPUMove(game);
                }
                if (spStatus != 3)
                    spGamePrintBoard(game);
                printf("%s player - enter your move:\n",
                       game->settings->curr_turn == SP_USER_COLOR_WHITE ? "white" : "black");
                fgets(s, SP_MAX_LINE_LENGTH, stdin);
                if (s == NULL) {
                    printf("Error: fgets has failed\n");
                    return 0;
                }
                s[strcspn(s, "\r\n")] = 0;
                SPGameCommand cmd = spGameParser(s);
                spStatus = spGameHandler(game, cmd);
            }
        } while (spStatus != -1);
        spGameDestroy(game);
        return EXIT_SUCCESS;
    }
    return 0;
}