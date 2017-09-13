CC = gcc
OBJS = SPMiniMaxNode.o SPMiniMax.o SPGame.o SPTileArrayList.o SPParser.o SPMainAux.o main.o
EXEC = SP
GAME_TEST_OBJS = SPTileArrayList.o SPGame.o SPGameUnitTest.o
ARRAY_LIST_TEST_OBJS = SPArrayListUnitTest.o SPTileArrayList.o
PARSER_TEST_OBJS = SPParser.o SPParserUnitTest.o
MINMAXNode_TEST_OBJ = SPMiniMaxNode.o SPMiniMaxNodeUnitTest.o SPGame.o SPTileArrayList.o
MINMAX_TEST_OBJ = SPMiniMaxNode.o SPMiniMax.o SPMiniMaxUnitTest.o SPGame.o SPTileArrayList.o
MAINAUX_TEST_OBJS = SPMainAux.o SPTileArrayList.o SPGame.o SPMiniMax.o SPMiniMaxNode.o
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
UNIT_TESTS = SPGameUnitTest SPArrayListUnitTest SPParser SPMiniMaxUnitTest\
SPMiniMaxNodeUnitTest SPMainAuxUnitTest

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
SPGameUnitTest: $(GAME_TEST_OBJS)
	$(CC) $(GAME_TEST_OBJS) -o $@
SPArrayListUnitTest: $(ARRAY_LIST_TEST_OBJS)
	$(CC) $(ARRAY_LIST_TEST_OBJS) -o $@
SPParserUnitTest: $(PARSER_TEST_OBJS)
	$(CC) $(PARSER_TEST_OBJS) -o $@
SPMiniMaxUnitTest: $(MINMAX_TEST_OBJ)
	$(CC) $(MINMAX_TEST_OBJ) -o $@
SPMiniMaxNodeUnitTest: $(MINMAXNode_TEST_OBJ)
	$(CC) $(MINMAXNode_TEST_OBJ) -o $@
SPMainAuxUnitTest: $(MAINAUX_TEST_OBJS)
	$(CC) $(MAINAUX_TEST_OBJS) -o $@
SPGameUnitTest.o: SPGameUnitTest.c unit_test_util.h SPTileArrayList.h SPGame.h
	$(CC) $(COMP_FLAG) -c $*.c
SPArrayListUnitTest.o: SPArrayListUnitTest.c SPTileArrayList.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
SPParserUnitTest.o: SPParserUnitTest.c SPParser.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMiniMaxUnitTest.o: SPMiniMaxUnitTest.c SPMiniMax.h SPGame.h SPMiniMaxNode.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMiniMaxNodeUnitTest.o: SPMiniMaxNodeUnitTest.c SPMiniMaxNode.h unit_test_util.h SPGame.h
	$(CC) $(COMP_FLAG) -c $*.c
SPTileArrayList.o: SPTileArrayList.h SPTileArrayList.c
	$(CC) $(COMP_FLAG) -c $*.c
SPGame.o: SPGame.c SPGame.h SPTileArrayList.h
	$(CC) $(COMP_FLAG) -c $*.c
SPParser.o: SPParser.c SPParser.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMiniMaxNode.o: SPMiniMaxNode.c SPMiniMaxNode.h SPGame.h
	$(CC) $(COMP_FLAG) -c $*.c 
SPMiniMax.o: SPMiniMax.c SPMiniMax.h SPMiniMaxNode.h SPGame.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMainAux.o: SPMainAux.c SPMainAux.h SPGame.h SPMiniMax.h
	$(CC) $(COMP_FLAG) -c $*.c
main.o: main.c SPMainAux.h SPGame.h SPMiniMax.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
