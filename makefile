#compiler and flags
CC = gcc
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags\
-lSDL2 -lSDL2main
VPATH = GUI
EXEC = chessprog

#objects for exec
CONSOLE_OBJS =SPGame.o SPMiniMax.o SPParser.o SPSettings.o SPQueue.o SPMoves.o
GUI_OBJS = SPGUIBasics.o SPGUIGameWindow.o SPGUILoadWindow.o SPGUIMainWindow.o SPGUIController.o SPGUISettingsWindow.o
ALL_OBJS = $(CONSOLE_OBJS) $(GUI_OBJS) main.o

#The exec file
all: $(ALL_OBJS)
	$(CC) $(ALL_OBJS) $(SDL_LIB) -o $(EXEC)
$(EXEC): $(ALL_OBJS)
	$(CC) $(ALL_OBJS) $(SDL_LIB) -o $@

#console rules
SPQueue.o: SPQueue.c SPQueue.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMoves.o: SPMoves.h SPMoves.c
	$(CC) $(COMP_FLAG) -c $*.c
SPParser.o: SPParser.c SPParser.h SPMoves.h
	$(CC) $(COMP_FLAG) -c $*.c
SPSettings.o: SPSettings.c SPSettings.h SPParser.h
	$(CC) $(COMP_FLAG) -c $*.c
SPGame.o: SPGame.c SPGame.h SPQueue.h SPMiniMax.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMiniMax.o: SPMiniMax.c SPMiniMax.h SPGame.h
	$(CC) $(COMP_FLAG) -c $*.c

#gui rules
GUI/SPGUIBasics.o: SPGUIBasics.c SPGUIBasics.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI/SPGUIMainWindow.o: SPGUIMainWindow.c SPGUIMainWindow.h SPGUIBasics.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI/SPGUILoadWindow.o: SPGUILoadWindow.c SPGUILoadWindow.h SPGUIBasics.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI/SPGUISettingsWindow.o: SPGUISettingsWindow.c SPGUISettingsWindow.h SPGUIBasics.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI/SPGUIGameWindow.o: SPGUIGameWindow.c SPGUIGameWindow.h SPGUIBasics.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GUI/SPGUIController.o: SPGUIController.c SPGUIController.h SPGUILoadWindow.h SPGUILoadWindow.h SPGUISettingsWindow.h SPGUIGameWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
	
#main rule
main.o: main.c SPGame.h SPGUIController.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
	
#clean rule	
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)