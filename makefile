CC = gcc
OBJS = main.o SPGame.o SPParser.o SPSettings.o SPQueue.o SPMoves.o SPMiniMax.o
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
SPQueue.o: SPQueue.h SPQueue.c 
	$(CC) $(COMP_FLAG) -c $*.c
SPSettings.o: SPSettings.h SPSettings.c SPParser.h
	$(CC) $(COMP_FLAG) -c $*.c
SPGame.o: SPGame.c SPGame.h SPMiniMax.h
	$(CC) $(COMP_FLAG) -c $*.c
SPParser.o: SPParser.c SPParser.h SPMoves.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMoves.o: SPMoves.c SPMoves.h
	$(CC) $(COMP_FLAG) -c $*.c 
SPMiniMax.o: SPMiniMax.c SPMiniMax.h SPGame.h
	$(CC) $(COMP_FLAG) -c $*.c
main.o: main.c SPGame.h SPParser.h
	$(CC) $(COMP_FLAG) -c $*.c
