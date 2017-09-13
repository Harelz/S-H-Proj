//
// Created by hoshri on 9/11/2017.
//

#ifndef UNTITLED1_SPMOVES_H
#define UNTITLED1_SPMOVES_H

#endif //UNTITLED1_SPMOVES_H

typedef struct{
    int row;
    int column;
} SPTile;

typedef struct {
    SPTile from;
    SPTile to;
} SPMove;

int getMoves(SPTile* tile);
int movePiece(char* command);