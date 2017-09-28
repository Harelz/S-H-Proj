#include "SPGUIGameWindow.h"

SPGUIGameWindow* spGameWindowCreate(SPGame* gameCopy) {
	SPGUIGameWindow* res = (SPGUIGameWindow*) calloc(sizeof(SPGUIGameWindow),
			sizeof(char));
	SDL_Surface* loadingSurfaceWhite = NULL; //Used as temp surface
	SDL_Surface* loadingSurfaceBlack = NULL; //Used as temp surface
	SDL_Surface* loadingSurfaceGrid = NULL; //loading surface grid, Used as temp surface
	if (res == NULL) {
		printf("Couldn't create SPGUIGameWindow struct\n");
		return NULL;
	}
	res->gameWindow = SDL_CreateWindow("CHESS GAME: Game Window",
	SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, 850, 650, SDL_WINDOW_OPENGL);
	if (res->gameWindow == NULL) {
		spGameWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}

	res->gameRenderer = SDL_CreateRenderer(res->gameWindow, -1,
			SDL_RENDERER_ACCELERATED);
	if (res->gameRenderer == NULL) {
		spGameWindowDestroy(res);
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}

	//create panel array
	res->numOfPanel = NUM_OF_GAME_BUTTONS;
	const char* activeImages[NUM_OF_GAME_BUTTONS] = { ACTIVE_BMP_PATH(restart), ACTIVE_BMP_PATH(
			save), ACTIVE_BMP_PATH(load), ACTIVE_BMP_PATH(undo), ACTIVE_BMP_PATH(main_menu), ACTIVE_BMP_PATH(
			exit) };

	const char* inactiveImages[NUM_OF_GAME_BUTTONS] = { ACTIVE_BMP_PATH(restart),
			INACTIVE_BMP_PATH(save), INACTIVE_BMP_PATH(load), INACTIVE_BMP_PATH(undo), INACTIVE_BMP_PATH(
					main_menu), INACTIVE_BMP_PATH(exit) };

	int xBtns[NUM_OF_GAME_BUTTONS] = { 25, 25, 25, 25, 25, 25 };
	int yBtns[NUM_OF_GAME_BUTTONS] = { 40, 120, 200, 280, 490, 570 };

	bool visible[NUM_OF_GAME_BUTTONS] = { true, true, true, true, true, true };

	bool active[NUM_OF_GAME_BUTTONS] = { true, true, true, false, true, true };

	SPGUI_BUTTON_TYPE types[NUM_OF_GAME_BUTTONS] = { BUTTON_GAME_RESTART,
			BUTTON_GAME_SAVE, BUTTON_GAME_LOAD, BUTTON_GAME_UNDO,
			BUTTON_GAME_MAIN_MENU, BUTTON_GAME_EXIT };

	res->panel = createButtons(res->gameRenderer, activeImages, inactiveImages,
			xBtns, yBtns, visible, active, types, res->numOfPanel);

	if (res->panel == NULL) {
		SDL_DestroyRenderer(res->gameRenderer);
		SDL_DestroyWindow(res->gameWindow);
		free(res);
	}
	//Create a chess_board texture:
	loadingSurfaceGrid = SDL_LoadBMP(ACTIVE_BMP_PATH(chess_grid));
	if (loadingSurfaceGrid == NULL) {
		printf("Could not create a surface: %s\n", SDL_GetError());
		return NULL;
	}
	res->chessGrid = SDL_CreateTextureFromSurface(res->gameRenderer,
			loadingSurfaceGrid);
	if (res->chessGrid == NULL) {
		printf("Could not create a texture: %s\n", SDL_GetError());
		SDL_DestroyTexture(res->chessGrid);
		SDL_DestroyRenderer(res->gameRenderer);
		SDL_DestroyWindow(res->gameWindow);
		free(res);
		return NULL;
	}
	SDL_FreeSurface(loadingSurfaceGrid); //We finished with the surface -> delete it
	//create white & black pieces
	const char* whiteImages[NUM_OF_DIFF_PIECES] = { ACTIVE_BMP_PATH(white_pawn),
			ACTIVE_BMP_PATH(white_knight), ACTIVE_BMP_PATH(white_bishop), ACTIVE_BMP_PATH(white_rook),
			ACTIVE_BMP_PATH(white_queen), ACTIVE_BMP_PATH(white_king) };

	const char* blackImages[NUM_OF_DIFF_PIECES] = { ACTIVE_BMP_PATH(black_pawn),
			ACTIVE_BMP_PATH(black_knight), ACTIVE_BMP_PATH(black_bishop), ACTIVE_BMP_PATH(black_rook),
			ACTIVE_BMP_PATH(black_queen), ACTIVE_BMP_PATH(black_king) };

	for (int i = 0; i < NUM_OF_DIFF_PIECES; i++) {
		loadingSurfaceWhite = SDL_LoadBMP(whiteImages[i]);
		loadingSurfaceBlack = SDL_LoadBMP(blackImages[i]);
		if (loadingSurfaceWhite == NULL || loadingSurfaceBlack == NULL) {
			printf("Could not create a surface: %s\n", SDL_GetError());
			SDL_FreeSurface(loadingSurfaceWhite);
			SDL_FreeSurface(loadingSurfaceBlack);
			for (int k = 0; k < i; k++) {
				SDL_DestroyTexture(res->whitePieces[k]);
				SDL_DestroyTexture(res->blackPieces[k]);
			}
			SDL_DestroyTexture(res->chessGrid);
			destroyButtons(res->panel, res->numOfPanel);
			SDL_DestroyRenderer(res->gameRenderer);
			SDL_DestroyWindow(res->gameWindow);
			free(res);
			return NULL;
		}
		res->whitePieces[i] = SDL_CreateTextureFromSurface(res->gameRenderer,
				loadingSurfaceWhite);
		res->blackPieces[i] = SDL_CreateTextureFromSurface(res->gameRenderer,
				loadingSurfaceBlack);
		if (res->whitePieces[i] == NULL || res->blackPieces[i] == NULL) {
			printf("Could not create a texture: %s\n", SDL_GetError());
			SDL_DestroyTexture(res->whitePieces[i]);
			SDL_DestroyTexture(res->blackPieces[i]);
			for (int k = 0; k < i; k++) {
				SDL_DestroyTexture(res->whitePieces[k]);
				SDL_DestroyTexture(res->blackPieces[k]);
			}
			SDL_DestroyTexture(res->chessGrid);
			destroyButtons(res->panel, res->numOfPanel);
			SDL_DestroyRenderer(res->gameRenderer);
			SDL_DestroyWindow(res->gameWindow);
			free(res);
			return NULL;
		}
		SDL_FreeSurface(loadingSurfaceWhite);
		SDL_FreeSurface(loadingSurfaceBlack);
	}
	res->game = spGameCopy(gameCopy); //copy the of the game sent
	res->isSaved = false; //the game is not saved yet
	res->chosenPiece[0] = -1;
	res->chosenPiece[1] = -1;
	return res;
}

void spGameWindowDestroy(SPGUIGameWindow* src) {
	if (!src)
		return;

	if (src->game != NULL)
		spGameDestroy(src->game);

	if (src->panel != NULL)
		destroyButtons(src->panel, src->numOfPanel);

	if (src->chessGrid != NULL)
		SDL_DestroyTexture(src->chessGrid);

	for (int i = 0; i < NUM_OF_DIFF_PIECES; i++) {
		if (src->whitePieces[i] != NULL && src->blackPieces[i] != NULL) {
			SDL_DestroyTexture(src->whitePieces[i]);
			SDL_DestroyTexture(src->blackPieces[i]);
		}
	}
	if (src->gameRenderer != NULL)
		SDL_DestroyRenderer(src->gameRenderer);

	if (src->gameWindow != NULL)
		SDL_DestroyWindow(src->gameWindow);

	free(src);
}

void spGameWindowDraw(SPGUIGameWindow* src, SDL_Event* event) {
	if (src == NULL)
		return;

	SDL_SetRenderDrawColor(src->gameRenderer, 169, 169, 169, 255);
	SDL_RenderClear(src->gameRenderer);

	for (int i = 0; i < src->numOfPanel; i++)
		drawButton(src->panel[i]);

	SDL_Rect rec = { .x = PANEL_OFFSET, .y = 0, .w = GUI_BOARD_SIZE, .h =
	GUI_BOARD_SIZE }; //rect of chess_grid
	SDL_RenderCopy(src->gameRenderer, src->chessGrid, NULL, &rec);

	//fill the board acoording to pieces
	for (int i = 0; i < SP_GAMEBOARD_SIZE; i++) {
		for (int j = 0; j < SP_GAMEBOARD_SIZE; j++) {
			if (i == src->chosenPiece[0] && j == src->chosenPiece[1]) //the chosen piece needs not to be drawn at his location
				continue;
			else {
				rec.x = (PANEL_OFFSET + j * (GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE)) + 5;
				rec.y = ((GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE) * (SP_GAMEBOARD_SIZE - 1 - i))
						+ 5;
				rec.h = ((GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE) - 10);
				rec.w = ((GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE) - 10);
				if (src->game->gameBoard[i][j] != SP_GAME_EMPTY_ENTRY)
					drawPieceByEntry(src, rec, i, j);
			}

		}
	}
	if (event != NULL && src->chosenPiece[0] != -1) { //piece was selected, move it with mouse
		int mouseX = event->motion.x;
		int mouseY = event->motion.y;
		rec.x = mouseX - (int) ((GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE) / 2); //piece is centered according to mouse
		rec.y = mouseY - (int) ((GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE) / 2);
		rec.h = (GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE) - 15;
		rec.w = (GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE) - 15;
		drawPieceByEntry(src, rec, src->chosenPiece[0], src->chosenPiece[1]);
	}
	SDL_RenderPresent(src->gameRenderer);
}

void drawPieceByEntry(SPGUIGameWindow* src, SDL_Rect rec, int i, int j) {
	if (!src)
		return;
	char piece = src->game->gameBoard[i][j];
	switch (piece) {
	case W_PAWN:
		SDL_RenderCopy(src->gameRenderer, src->whitePieces[0], NULL, &rec);
		break;
	case B_PAWN:
		SDL_RenderCopy(src->gameRenderer, src->blackPieces[0], NULL, &rec);
		break;
	case W_BISHOP:
		SDL_RenderCopy(src->gameRenderer, src->whitePieces[2], NULL, &rec);
		break;
	case B_BISHOP:
		SDL_RenderCopy(src->gameRenderer, src->blackPieces[2], NULL, &rec);
		break;
	case W_ROOK:
		SDL_RenderCopy(src->gameRenderer, src->whitePieces[3], NULL, &rec);
		break;
	case B_ROOK:
		SDL_RenderCopy(src->gameRenderer, src->blackPieces[3], NULL, &rec);
		break;
	case W_KNIGHT:
		SDL_RenderCopy(src->gameRenderer, src->whitePieces[1], NULL, &rec);
		break;
	case B_KNIGHT:
		SDL_RenderCopy(src->gameRenderer, src->blackPieces[1], NULL, &rec);
		break;
	case W_QUEEN:
		SDL_RenderCopy(src->gameRenderer, src->whitePieces[4], NULL, &rec);
		break;
	case B_QUEEN:
		SDL_RenderCopy(src->gameRenderer, src->blackPieces[4], NULL, &rec);
		break;
	case W_KING:
		SDL_RenderCopy(src->gameRenderer, src->whitePieces[5], NULL, &rec);
		break;
	case B_KING:
		SDL_RenderCopy(src->gameRenderer, src->blackPieces[5], NULL, &rec);
		break;
	}
}

SPGUI_GAME_EVENT spGameWindowEventHandler(SPGUIGameWindow *src, SDL_Event *event) {
	if (!src || !event)
		return SPGUI_GAME_INVALID_ARG;
	if (src->game->settings->game_mode == SP_MODE_2P)
		src->panel[3]->active = false; // undo button deactivated
	else if (!spQueueIsEmpty(src->game->history))
		src->panel[3]->active = true; // undo button activated
	if (src->game->settings->game_mode == SP_MODE_1P &&
		src->game->settings->p1_color == BLACK && src->game->settings->curr_turn == WHITE) {
		spGameWindowDraw(src, event);
		spSetNaiveCPUMove(src->game);
		SPGUI_GAME_EVENT msg = checkStatusForUserGui(src);
		if (spStatusAfterMove(msg, src, event) != SPGUI_GAME_NONE)
			return msg;
	}
	if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
		if (event->button.x > PANEL_OFFSET) { //drag n drop
			int from[2] = {event->button.x, event->button.y};
			computeLocFromGui(from); //change from - gui location to console location
			if (event->type == SDL_MOUSEBUTTONDOWN
				&& getColor(src->game->gameBoard[from[0]][from[1]]) == src->game->settings->curr_turn) {
				src->chosenPiece[0] = from[0];
				src->chosenPiece[1] = from[1];
			} else if (event->type == SDL_MOUSEBUTTONUP && src->chosenPiece[0] != -1) {
				//check if legal move, change chosenPiece check undo checkCheck, not saved = true
				int to[2] = {event->button.x, event->button.y};
				computeLocFromGui(to);
				SPMove *myMove = spCreateMove(src->chosenPiece[0], src->chosenPiece[1], to[0], to[1]);
                int status;
				if ((status = spGameMoveHandler(src->game, myMove)) != 3) { // need to check if success
					src->isSaved = false;
					src->chosenPiece[0] = -1;
					src->chosenPiece[1] = -1;
                    if (status == -1)
                        changeColor(src->game);
					SPGUI_GAME_EVENT msg = checkStatusForUserGui(src);
					if (spStatusAfterMove(msg, src, event) != SPGUI_GAME_NONE) {
						spDestroyMove(myMove);
						return msg;
					}
					/*//computer turn (if computer is black)
					if (src->game->settings->game_mode == 1&& src->game->settings->p1_color == 1
						&& src->game->settings->curr_turn == BLACK) {
						spGameWindowDraw(src, event);
						SPMove* compMove = spCreateMove(0,0,0,0);
						spMinimaxSuggestMove(src->game,compMove);
						spGameMoveHandler(src->game, compMove);
						spDestroyMove(compMove);
						SPGUI_GAME_EVENT msg = checkStatusForUserGui(src);
						if (spStatusAfterMove(msg, src, event) != SPGUI_GAME_NONE){
							spDestroyMove(myMove);
							return msg;}
					}*/
					spDestroyMove(myMove);
					return SPGUI_GAME_MOVE;
				}
				src->chosenPiece[0] = -1;
				src->chosenPiece[1] = -1;
				spDestroyMove(myMove);
				return SPGUI_GAME_NONE;
			}
		} else if (event->type == SDL_MOUSEBUTTONUP)
			return spPanelHandleEvent(src, event);
	} else if (event->type == SDL_MOUSEMOTION
				   && (src->chosenPiece[0] == -1 || event->button.x < PANEL_OFFSET)) {
			src->chosenPiece[0] = -1;
			src->chosenPiece[1] = -1;
	} else if (event->type == SDL_WINDOWEVENT) {
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SPGUI_GAME_QUIT;
	}
		return SPGUI_GAME_NONE;
	}
SPGUI_GAME_EVENT checkStatusForUserGui(SPGUIGameWindow* src) { // edited
	if (spGameIsMate(src->game)) {
		if (src->game->settings->curr_turn == BLACK) // Black Player Wins!
			return SPGUI_GAME_PLAYER_1_CHECKMATE;
		else { //White Player Wins!
			return SPGUI_GAME_PLAYER_2_CHECKMATE;
		}
	}
	char playerCheck = spGameIsCheck(src->game);
	if (playerCheck != SP_GAME_EMPTY_ENTRY) {
		if (src->game->settings->curr_turn == WHITE && (playerCheck == WHITE || playerCheck == SP_GAME_COLOR_BOTH))
			return SPGUI_GAME_PLAYER_1_CHECK;
		else if (src->game->settings->curr_turn == BLACK && (playerCheck == BLACK || playerCheck == SP_GAME_COLOR_BOTH))
			return SPGUI_GAME_PLAYER_2_CHECK;
	}
	if (spGameIsTie(src->game))
		return SPGUI_GAME_TIE;
	return SPGUI_GAME_MOVE;

}

SPGUI_GAME_EVENT spStatusAfterMove(SPGUI_GAME_EVENT msg, SPGUIGameWindow* src, SDL_Event* event) {
	if (msg == SPGUI_GAME_PLAYER_1_CHECKMATE
			|| msg == SPGUI_GAME_PLAYER_2_CHECKMATE
			|| msg == SPGUI_GAME_TIE) { //terminal state
		return msg;
	} else if (msg == SPGUI_GAME_PLAYER_1_CHECK
			|| msg == SPGUI_GAME_PLAYER_2_CHECK) {
		//draw the board before show "check" msg
		spGameWindowDraw(src, event);
		if (msg == SPGUI_GAME_PLAYER_1_CHECK)
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECK!",
					"White king is threatend!", NULL);
		else
			//msg == SPGUI_GAME_PLAYER_2_CHECK
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECK!",
					"Black king is threatend!", NULL);
	}
	return SPGUI_GAME_NONE;
}

SPGUI_GAME_EVENT spPanelHandleEvent(SPGUIGameWindow* src, SDL_Event* event) {
	SPGUI_BUTTON_TYPE btn = NO_BUTTON;
	btn = getButtonClicked(src->panel, src->numOfPanel, event, true);
	if (btn == BUTTON_GAME_RESTART) {
		spGameDestroy(src->game);
		src->game = spGameCreateDef();//restart the game with current settings
		src->panel[3]->active = false; //de-activate undo btn
		src->isSaved = false;
		return SPGUI_GAME_RESTART;
	} else if (btn == BUTTON_GAME_SAVE) {
		promoteSlots();
		if (spGameSaveHandler(src->game, SAVE1) == -1) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Unable to save game", NULL);
			return SPGUI_GAME_NONE;
		} else
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SUCCESS", "game was successfully saved!", NULL);
		src->isSaved = true;
		return SPGUI_GAME_SAVE;
	} else if (btn == BUTTON_GAME_LOAD) {
		return SPGUI_GAME_LOAD;
	} else if (btn == BUTTON_GAME_UNDO) {
		spGameUndoHandler(src->game);
		//if there is no history, de-activate undo btn
		if (spQueueIsEmpty(src->game->history))
			src->panel[3]->active = false;
		src->isSaved = false;
		return SPGUI_GAME_UNDO;
	} else if (btn == BUTTON_GAME_MAIN_MENU) {
		if (src->isSaved == false) {
			//show SDL_ShowMessageBox asking the user if he wants to save
			int choice = popUpSave();
			if (choice != 2) {
				if (choice == 1) { //want to save
					promoteSlots();
					if (spGameSaveHandler(src->game, SAVE1) == -1) {
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR",
								"Unable to save game",
								NULL);
						return SPGUI_GAME_NONE;
					} else
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
								"SUCCESS", "game was successfully saved!",
								NULL);
					src->isSaved = true;
				}
				return SPGUI_GAME_MAIN_MENU;
			} else
				//popUpSave() == 2
				return SPGUI_GAME_NONE;
		}
		return SPGUI_GAME_MAIN_MENU;
	} else if (btn == BUTTON_GAME_EXIT) {
		if (src->isSaved == false) {
			//show SDL_ShowMessageBox asking the user if he wants to save
			int choice = popUpSave();
			if (choice != 2) {
				if (choice == 1) { //want to save
					promoteSlots();
					if (spGameSaveHandler(src->game, SAVE1) == -1) {
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR",
								"Unable to save game",
								NULL);
						return SPGUI_GAME_NONE;
					} else
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
								"SUCCESS", "game was successfully saved!",
								NULL);

					src->isSaved = true;
				}
				return SPGUI_GAME_EXIT;
			} else
				//popUpSave() == 2
				return SPGUI_GAME_NONE;
		}
		return SPGUI_GAME_EXIT;
	}
	return SPGUI_GAME_NONE;
}

int popUpSave() {
	const SDL_MessageBoxButtonData buttons[] = { {
			SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" }, { 0, 0, "no" },
			{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" }, };
	const SDL_MessageBoxColorScheme colorScheme = { { { 255, 0, 0 },
			{ 0, 255, 0 }, { 255, 255, 0 }, { 0, 0, 255 }, { 255, 0, 255 } } };
	const SDL_MessageBoxData messageboxdata = { SDL_MESSAGEBOX_INFORMATION,
	NULL, "The game isn't saved!",
			"Do you wish to save the game before leaving?", SDL_arraysize(
					buttons), buttons, &colorScheme };
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		SDL_Log("error displaying message box");
		return 0; /* error */
	}
	if (buttonid == -1 || buttonid == 2) {
		return 2;
	} else if (buttonid == 1)
		return 1;

	return 0;
}

void computeLocFromGui(int loc[2]) {
	int tmp = loc[0];
	loc[0] = (SP_GAMEBOARD_SIZE - 1) - (int) (loc[1] / (GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE));
	loc[1] = (int) ((tmp - PANEL_OFFSET) / (GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE));
}

