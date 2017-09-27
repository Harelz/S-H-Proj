#include "SPCHESSGUIGameWin.h"

SPGameWin* spGameWindowCreate(SPGame* gameCopy) {
	SPGameWin* res = (SPGameWin*) calloc(sizeof(SPGameWin),
			sizeof(char));
	SDL_Surface* loadingSurfaceWhite = NULL; //Used as temp surface
	SDL_Surface* loadingSurfaceBlack = NULL; //Used as temp surface
	SDL_Surface* loadingSurfaceGrid = NULL; //loading surface grid, Used as temp surface
	if (res == NULL) {
		printf("Couldn't create SPGameWin struct\n");
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
	const char* activeImages[NUM_OF_GAME_BUTTONS] = { ACT_IMG(restart), ACT_IMG(
			save), ACT_IMG(load), ACT_IMG(undo), ACT_IMG(main_menu), ACT_IMG(
			exit) };

	const char* inactiveImages[NUM_OF_GAME_BUTTONS] = { ACT_IMG(restart),
			INACT_IMG(save), INACT_IMG(load), INACT_IMG(undo), INACT_IMG(
					main_menu), INACT_IMG(exit) };

	int xBtns[NUM_OF_GAME_BUTTONS] = { 25, 25, 25, 25, 25, 25 };
	int yBtns[NUM_OF_GAME_BUTTONS] = { 40, 120, 200, 280, 490, 570 };

	bool visible[NUM_OF_GAME_BUTTONS] = { true, true, true, true, true, true };

	bool active[NUM_OF_GAME_BUTTONS] = { true, true, true, false, true, true };

	SPCHESS_BUTTON_TYPE types[NUM_OF_GAME_BUTTONS] = { BUTTON_GAME_RESTART,
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
	loadingSurfaceGrid = SDL_LoadBMP(ACT_IMG(chess_grid));
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
	const char* whiteImages[NUM_OF_DIFF_PIECES] = { ACT_IMG(white_pawn),
			ACT_IMG(white_knight), ACT_IMG(white_bishop), ACT_IMG(white_rook),
			ACT_IMG(white_queen), ACT_IMG(white_king) };

	const char* blackImages[NUM_OF_DIFF_PIECES] = { ACT_IMG(black_pawn),
			ACT_IMG(black_knight), ACT_IMG(black_bishop), ACT_IMG(black_rook),
			ACT_IMG(black_queen), ACT_IMG(black_king) };

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

void spGameWindowDestroy(SPGameWin* src) {
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

void spGameWindowDraw(SPGameWin* src, SDL_Event* event) {
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

void drawPieceByEntry(SPGameWin* src, SDL_Rect rec, int i, int j) {
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

SPCHESS_GAME_EVENT spGameWindowHandleEvent(SPGameWin* src,
		SDL_Event* event) {
	if (!src || !event)
		return SPCHESS_GAME_INVALID_ARG; //check to return invalid argument

	//check game state for undo btn
	if (src->game->settings->game_mode == 2)
		src->panel[3]->active = false; //game mode is 2, de-activate undo btn
	else if (!spQueueIsEmpty(src->game->history))//->movesPlayer1)
			//&& !spMovesListIsEmpty(src->game)//->movesPlayer2))
		src->panel[3]->active = true; //there is history, activate undo btn

	//computer turn (if computer is white)
	if (src->game->settings->game_mode
			== 1&& src->game->settings->p1_color == 0 && src->game->settings->curr_turn == WHITE) {
		spGameWindowDraw(src, event);
		SPMove* compMove =  spCreateMove(0,0,0,0);
		spMinimaxSuggestMove(src->game,compMove);
		spGameSetMove(src->game, compMove);
		spDestroyMove(compMove);
		SPCHESS_GAME_EVENT msg = checkStatusForUserGui(src);
		if (spStatusAfterMove(msg, src, event) != SPCHESS_GAME_NONE)
			return msg;
	}

	if (event->type == SDL_MOUSEBUTTONDOWN
			|| event->type == SDL_MOUSEBUTTONUP) {
		if (isClickOnBoard(event->button.x)) { //drag n drop
			int from[2] = { event->button.x, event->button.y };
			computeLocFromGui(from); //change from - gui location to console location
			if (event->type == SDL_MOUSEBUTTONDOWN
					&& getColor(src->game->gameBoard[from[0]][from[1]])==
							src->game->settings->curr_turn) {
				src->chosenPiece[0] = from[0];
				src->chosenPiece[1] = from[1];
			} else if (event->type == SDL_MOUSEBUTTONUP
					&& src->chosenPiece[0] != -1) {
				//check if legal move, change chosePiece check undo checkcheck, not saved = true
				int to[2] = { event->button.x, event->button.y };
				computeLocFromGui(to);
				if (!0)//SPGameIsKingRisker(src->game, src->chosenPiece, to) //TODO: look here.
						//&& spGameSetMove(src->game, src->chosenPiece, to)
								//== SP_GAME_SUCCESS) {
					src->isSaved = false;
					src->chosenPiece[0] = -1;
					src->chosenPiece[1] = -1;
					SPCHESS_GAME_EVENT msg = checkStatusForUserGui(src);
					if (spStatusAfterMove(msg, src, event) != SPCHESS_GAME_NONE) {
						return msg;
					}
					//computer turn (if computer is black)
					if (src->game->settings->game_mode
							== 1&& src->game->settings->p1_color == 1 && src->game->settings->curr_turn == BLACK) {
						spGameWindowDraw(src, event);
						SPMove* compMove = spCreateMove(0,0,0,0);
						spMinimaxSuggestMove(src->game,compMove);
						spGameSetMove(src->game, compMove);
						spDestroyMove(compMove);
						SPCHESS_GAME_EVENT msg = checkStatusForUserGui(src);
						if (spStatusAfterMove(msg, src, event) != SPCHESS_GAME_NONE)
							return msg;
					}
					return SPCHESS_GAME_MOVE;
				}
				src->chosenPiece[0] = -1;
				src->chosenPiece[1] = -1;
				return SPCHESS_GAME_NONE;
			}
		} else if (event->type == SDL_MOUSEBUTTONUP) {
			return spPanelHandleEvent(src, event);
		}
		 else if (event->type == SDL_MOUSEMOTION
			&& (src->chosenPiece[0] == -1 || !isClickOnBoard(event->button.x))) {
		src->chosenPiece[0] = -1;
		src->chosenPiece[1] = -1;
	} else if (event->type == SDL_WINDOWEVENT) {
		if (event->window.event == SDL_WINDOWEVENT_CLOSE)
			return SPCHESS_GAME_QUIT;
	}
	return SPCHESS_GAME_NONE;
}

SPCHESS_GAME_EVENT checkStatusForUserGui(SPGameWin* src) {
	char winner = SPGameCheckWinner(src->game);
	if (winner != '\0') {
		if (winner == WHITE)
			return SPCHESS_GAME_PLAYER_1_CHECKMATE;
		else { //winner == BLACK
			return SPCHESS_GAME_PLAYER_2_CHECKMATE;
		}
	}

	char whoisincheck = spChessIfMate(src->game);
	if (whoisincheck != '\0') {
		if (whoisincheck == WHITE)
			return SPCHESS_GAME_PLAYER_1_CHECK;
		else
			// whoisincheck == BLACK
			return SPCHESS_GAME_PLAYER_2_CHECK;
	}
	if (spGameIsTie(src->game))
		return SPCHESS_GAME_TIE;

	return SPCHESS_GAME_MOVE;

}

SPCHESS_GAME_EVENT spStatusAfterMove(SPCHESS_GAME_EVENT msg, SPGameWin* src, SDL_Event* event) {
	if (msg == SPCHESS_GAME_PLAYER_1_CHECKMATE
			|| msg == SPCHESS_GAME_PLAYER_2_CHECKMATE
			|| msg == SPCHESS_GAME_TIE) { //terminal state
		return msg;
	} else if (msg == SPCHESS_GAME_PLAYER_1_CHECK
			|| msg == SPCHESS_GAME_PLAYER_2_CHECK) {
		//draw the board before show "check" msg
		spGameWindowDraw(src, event);
		if (msg == SPCHESS_GAME_PLAYER_1_CHECK)
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECK!",
					"White king is threatend!", NULL);
		else
			//msg == SPCHESS_GAME_PLAYER_2_CHECK
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "CHECK!",
					"Black king is threatend!", NULL);
	}
	return SPCHESS_GAME_NONE;
}

SPCHESS_GAME_EVENT spPanelHandleEvent(SPGameWin* src, SDL_Event* event) {
	SPCHESS_BUTTON_TYPE btn = NO_BUTTON;

	btn = getButtonClicked(src->panel, src->numOfPanel, event,
	true);

	if (btn == BUTTON_GAME_RESTART) {
		spGameDestroy(src->game);
		src->game = spGameCreateDef();//restart the game with current settings
		src->panel[3]->active = false; //de-activate undo btn
		src->isSaved = false;
		return SPCHESS_GAME_RESTART;
	} else if (btn == BUTTON_GAME_SAVE) {
		promoteSlots();
		if (spGameSaveHandler(src->game, SLOT0) == -1) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR",
					"Unable to save game",
					NULL);
			return SPCHESS_GAME_NONE;
		} else
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SUCCESS",
					"game was successfully saved!", NULL);

		src->isSaved = true;
		return SPCHESS_GAME_SAVE;
	} else if (btn == BUTTON_GAME_LOAD) {
		return SPCHESS_GAME_LOAD;

	} else if (btn == BUTTON_GAME_UNDO) {
		spGameUndoHandler(src->game);
		spGameUndoHandler(src->game);
		//if there is no history, de-activate undo btn
		if (spQueueIsEmpty(src->game->history))
			src->panel[3]->active = false;
		src->isSaved = false;
		return SPCHESS_GAME_UNDO;
	} else if (btn == BUTTON_GAME_MAIN_MENU) {
		if (src->isSaved == false) {
			//show SDL_ShowMessageBox asking the user if he wants to save
			int choice = popUpSave();
			if (choice != 2) {
				if (choice == 1) { //want to save
					promoteSlots();
					if (spGameSaveHandler(src->game, SLOT0) == -1) {
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR",
								"Unable to save game",
								NULL);
						return SPCHESS_GAME_NONE;
					} else
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
								"SUCCESS", "game was successfully saved!",
								NULL);
					src->isSaved = true;
				}
				return SPCHESS_GAME_MAIN_MENU;
			} else
				//popUpSave() == 2
				return SPCHESS_GAME_NONE;
		}
		return SPCHESS_GAME_MAIN_MENU;
	} else if (btn == BUTTON_GAME_EXIT) {
		if (src->isSaved == false) {
			//show SDL_ShowMessageBox asking the user if he wants to save
			int choice = popUpSave();
			if (choice != 2) {
				if (choice == 1) { //want to save
					promoteSlots();
					if (spGameSaveHandler(src->game, SLOT0) == -1) {
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR",
								"Unable to save game",
								NULL);
						return SPCHESS_GAME_NONE;
					} else
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
								"SUCCESS", "game was successfully saved!",
								NULL);

					src->isSaved = true;
				}
				return SPCHESS_GAME_EXIT;
			} else
				//popUpSave() == 2
				return SPCHESS_GAME_NONE;
		}
		return SPCHESS_GAME_EXIT;
	}
	return SPCHESS_GAME_NONE;
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

bool isClickOnBoard(int x) {
	return (x > PANEL_OFFSET);
}

void computeLocFromGui(int loc[2]) {
	int tmp = loc[0];
	loc[0] = (SP_GAMEBOARD_SIZE - 1) - (int) (loc[1] / (GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE));
	loc[1] = (int) ((tmp - PANEL_OFFSET) / (GUI_BOARD_SIZE / SP_GAMEBOARD_SIZE));
}

void spGameWindowHide(SPGameWin* src) {
	SDL_HideWindow(src->gameWindow);
}

void spGameWindowShow(SPGameWin* src) {
	SDL_ShowWindow(src->gameWindow);
}

