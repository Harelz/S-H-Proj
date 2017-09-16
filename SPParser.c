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

SPGameCommand spGameParser(const char* str){
	char s[SP_MAX_LINE_LENGTH];
	strcpy(s,str);
	SPGameCommand spcmd;
	if (s == NULL){
		spcmd.cmd = SP_GINVALID_LINE;
		return spcmd;
	}
	char* token = strtok(s, " \t\r\n");
	if(token == NULL){
		spcmd.cmd = SP_GINVALID_LINE;
		return spcmd;
	}
	else if (strcmp(token,"move") == 0)
		spcmd.cmd = SP_MOVE;
	else if (strcmp(token,"castle") == 0)
		spcmd.cmd = SP_CASTLE;
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
	token = strtok(NULL , " \t\r\n");
	if(token == NULL) {
		spcmd.arg = -1;
		return spcmd;
	}
	else if(spcmd.cmd == SP_SAVE) {
		spcmd.pathArg = token;
	}
	else if (spcmd.cmd == SP_GET_MOVES || spcmd.cmd == SP_CASTLE){
		SPTile* fromTile = spParseTile(token);
		if (fromTile == NULL)
			spcmd.cmd = SP_GINVALID_LINE;
		else
			spcmd.tile = fromTile;
        token = strtok(NULL , " \t\r\n");
        if (token != NULL)
            spcmd.cmd = SP_GINVALID_LINE;

	}
	else if (spcmd.cmd == SP_MOVE){
		SPTile* fromTile = spParseTile(token);
		token = strtok(NULL , " \t\r\n");
		char* s = token;
		token = strtok(NULL , " \t\r\n");
		SPTile* toTile = spParseTile(token);
		if (fromTile == NULL || strcmp(s,"to") != 0 || toTile == NULL)
			spcmd.cmd = SP_GINVALID_LINE;
		else{
			SPMove* totMove = spCreateMoveFromTile(fromTile, toTile);
			spcmd.move = totMove;
		}
        token = strtok(NULL , " \t\r\n");
        if (token != NULL)
            spcmd.cmd = SP_GINVALID_LINE;
	}
	else if(spcmd.cmd == SP_UNDO || spcmd.cmd == SP_RESET || spcmd.cmd == SP_GQUIT)
		spcmd.cmd = SP_GINVALID_LINE;
	CHECK_VALID(spcmd);
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
    }
	else if(spcmd.cmd == SP_START || spcmd.cmd == SP_SQUIT || spcmd.cmd == SP_PRINT ||spcmd.cmd == SP_DEFAULT)
		spcmd.cmd = SP_SINVALID_LINE;
    else if(spParserIsInt(token)){
		spcmd.arg = atoi(token);
    }
	CHECK_VALID(spcmd);
	return spcmd;
}



//TODO:ADDITION ######################### NOT PERMENANT


SPTile* spParseTile(char* s) { /*parse first pos in string s*/
    if (s == NULL)
        return NULL;
	SPTile* tile = spCreateTile(-1,-1);
	char *start, *mid, *end;
	start = strchr(s, '<');
	mid = strchr(s, ',');
	end = strchr(s, '>');

	if (start==NULL || mid==NULL || end==NULL || *(end+1) != '\0') /*illegal pos format*/
		return NULL;
	if (mid-start != 2 || end-mid != 2) {/* there should be precisely 1 char between each char*, end is really end*/
        return tile;
    }

	tile->row = *(start+1)-'1'; /* will check validity after return */
	tile->coloumn = *(mid+1)-'A';  /* will check validity after return */

	return tile;
}

/*

SPMove* spParseCastling(char *s) { //TODO: new - 2208
	SPTile tile;
	SPMove* move = NULL;
	SPArrayList* moves = NULL;

	s = skipSpaces(s + 7);
	tile = spParseTile(s);
	if (!isValidPos(tile)) */
/*case 1*//*

		0;//print_message(WRONG_POSITION);
	else if (!isUserPos(gameBoard, tile, userColor)) */
/*case 2*//*

		print_message(NO_DICS);
	else if (gameBoard[tile.column][tile.row] != W_ROOK && gameBoard[tile.column][tile.row] != B_ROOK) */
/*no rook*//*
 //TODO: this collides with case 3, verify error format
		print_message(NO_ROOK);
	else if ((moves = getCastlingMoves(gameBoard, userColor)) == NULL) { */
/*allocation error*//*

		startGame = 0; */
/*will exit game loop*//*

		print_malloc_error;
	}
	else { */
/*check if castling not possible for selected pos*//*

		for (SPArrayList* nextMove = moves; nextMove != NULL; nextMove = nextMove->to) */
/*either 0,1 or 2 moves*//*

			if (nextMove->from->from.column == tile.column && nextMove->from->from.row == tile.row) */
/*make sure it's not the other one*//*

				move = nextMove->from;
		freeListWithException(moves, move); */
/*keep good move if exists, otherwise delete all*//*

		if (move == NULL)
			print_message(ILLEGAL_CASTLING);
	}

	return move; */
/*will be NULL for all illegal values*//*

}

unsigned isValidPos(SPTile tile) { */
/*deals with case 1 in command move*//*

	if (tile.column<0 || tile.row<0 || tile.column>=SP_GAMEBOARD_SIZE || tile.row>=SP_GAMEBOARD_SIZE) */
/*outside board*//*

		return 0;
	return 1; //TODO: is that all for validity?
}

*/
/*@pre: pos is valid on board*//*

unsigned isUserPos(char board[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE], SPTile tile, char color) { */
/*deals with case 2 in command move*//*

	if (board[tile.column][tile.row]==SP_GAME_EMPTY_ENTRY)
		return 0;
	return color == getColor(board[tile.column][tile.row]);
}

int isPromotionMove(char board[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE], SPMove* move) {
	SPTile start=move->from, end=move->to;
	return  (start.row==6 && end.row==7 && board[start.row][start.column]==W_PAWN) ||
			(start.row==1 && end.row==0 && board[start.row][start.column]==B_PAWN);
}


*/
/*Will return 0, 1 or 2 possible castling moves in respect to color*//*

SPArrayList* getCastlingMoves(char board[SP_GAMEBOARD_SIZE][SP_GAMEBOARD_SIZE], char color){
	SPMove* move = NULL;
	SPArrayList* answer = NULL, *tmpList = NULL;
	SPTile tile;
	char lastMove[SP_GAMEBOARD_SIZE], k, r, l, kingPiece;
	int i, row;

	answer = initEmptyList();
	*/
/*Keeping last move *//*

	row = color == WHITE ? 0 : SP_GAMEBOARD_SIZE - 1;
	for (i = 0; i<SP_GAMEBOARD_SIZE; i++)
		lastMove[i] = board[i][row];

	*/
/*flags*//*
 //add those flags to game
	k = color == WHITE ? wk : bk; */
/*king*//*

	r = color == WHITE ? wrr : brr; */
/*right rook*//*

	l = color == WHITE ? wlr : blr; */
/*left rook*//*


	kingPiece = color == WHITE ? W_KING : B_KING;

	if (!k){
		*/
/*Right*//*

		if (!r && board[5][row] == SP_GAME_EMPTY_ENTRY && board[6][row] == SP_GAME_EMPTY_ENTRY && !isCheck(board, color)){ */
/*Empty & not Check*//*

			board[4][row] = SP_GAME_EMPTY_ENTRY;
			board[5][row] = kingPiece;
			if (!isCheck(board, color)){
				board[5][row] = SP_GAME_EMPTY_ENTRY;
				board[6][row] = kingPiece;
				if (!isCheck(board, color)){ */
/*a legal casteling move will be added*//*

					tile.column = 7;
					tile.row = row;
					if ((move = addPosToMove(move, tile)) == NULL) {
						freeList(answer);
						return NULL; */
/*allocation error*//*

					}
					tile.column = 5;
					tile.row = row;
					if ((addPosToMove(move, tile)) == NULL) {
						freeList(answer);
						freeMove(move);
						return NULL; */
/*allocation error*//*

					}
					move->special = move->to->special = CASTLE;
					if ((tmpList = addMoveToMoves(answer, move)) == NULL) {
						freeList(answer);
						freeMove(move);
						return NULL; */
/*allocation error*//*

					}

					answer = tmpList;
				}
			}
		}

		*/
/*Restoring the board for next move*//*

		for (i = 0; i<SP_GAMEBOARD_SIZE; i++)
			board[i][row] = lastMove[i];
		move = NULL;

		*/
/*Left*//*

		if (!l && board[2][row] == SP_GAME_EMPTY_ENTRY && board[3][row] == SP_GAME_EMPTY_ENTRY && !isCheck(board, color)){ */
/*Empty & not Check*//*

			board[4][row] = SP_GAME_EMPTY_ENTRY;
			board[3][row] = kingPiece;
			if (!isCheck(board, color)){
				board[3][row] = SP_GAME_EMPTY_ENTRY;
				board[2][row] = kingPiece;
				if (!isCheck(board, color)){
					tile.column = 0;
					tile.row = row;
					if ((move = addPosToMove(move, tile)) == NULL) { */
/*a legal casteling move will be added*//*

						freeList(answer);
						return NULL; */
/*allocation error*//*

					}
					tile.column = 3;
					tile.row = row;
					if ((addPosToMove(move, tile)) == NULL) {
						freeList(answer);
						freeMove(move);
						return NULL; */
/*allocation error*//*

					}
					move->special = move->to->special = CASTLE;
					if ((tmpList = addMoveToMoves(answer, move)) == NULL) {
						freeList(answer);
						freeMove(move);
						return NULL; */
/*allocation error*//*

					}

					answer = tmpList;
				}
			}
		}
	}


	*/
/*Restoring the board for next move*//*

	for (i = 0; i<SP_GAMEBOARD_SIZE; i++) */
/*For Castling - keeping the entire row*//*

		board[i][row] = lastMove[i];

	return answer;
}
*/
