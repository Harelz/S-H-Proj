#ifndef GRAPHICS_SPCHESSGUIBUTTON_H_
#define GRAPHICS_SPCHESSGUIBUTTON_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "../SPGame.h"

#define BUTTON_W 150
#define BUTTON_H 50
#define NUM_OF_SAVES 5
#ifdef _WIN32
#define ACTIVE_BMP_PATH(g) "../GUI/images/button_"#g".bmp"
#define INACTIVE_BMP_PATH(g) "../GUI/images/button_"#g"_F.bmp"
#define SAVE1 "../GUI/saved_games/save1.xml"
#define SAVE2 "../GUI/saved_games/save2.xml"
#define SAVE3 "../GUI/saved_games/save3.xml"
#define SAVE4 "../GUI/saved_games/save4.xml"
#define SAVE5 "../GUI/saved_games/save5.xml"
#elif __unix__
#define ACTIVE_BMP_PATH(g) "./GUI/images/button_"#g".bmp"
	#define INACTIVE_BMP_PATH(g) "./GUI/images/button_"#g"_F.bmp"
	#define SAVE1 "./GUI/saved_games/save1.xml"
	#define SAVE2 "./GUI/saved_games/save2.xml"
	#define SAVE3 "./GUI/saved_games/save3.xml"
	#define SAVE4 "./GUI/saved_games/save4.xml"
	#define SAVE5 "./GUI/saved_games/save5.xml"
#endif




/*
 * enum describes the diffrent buttons' type.
 */
typedef enum {
	//main window
	BUTTON_MAIN_NEW_GAME,
	BUTTON_MAIN_LOAD,
	BUTTON_MAIN_EXIT,

	//load window
	BUTTON_LOAD_SLOT0,
	BUTTON_LOAD_SLOT1,
	BUTTON_LOAD_SLOT2,
	BUTTON_LOAD_SLOT3,
	BUTTON_LOAD_SLOT4,
	BUTTON_LOAD_LOAD,
	BUTTON_LOAD_BACK,

	//set window
	BUTTON_SET_GAME_MODE,
	BUTTON_SET_ONE_PLAYER,
	BUTTON_SET_TWO_PLAYER,
	BUTTON_SET_DIFF,
	BUTTON_SET_NOOB_DIFF,
	BUTTON_SET_EASY_DIFF,
	BUTTON_SET_MOD_DIFF,
	BUTTON_SET_HARD_DIFF,
	BUTTON_SET_SELECT_COLOR,
	BUTTON_SET_BLACK_PLAYER,
	BUTTON_SET_WHITE_PLAYER,
	BUTTON_SET_BACK,
	BUTTON_SET_START,

	//game window
	BUTTON_GAME_RESTART,
	BUTTON_GAME_SAVE,
	BUTTON_GAME_LOAD,
	BUTTON_GAME_UNDO,
	BUTTON_GAME_MAIN_MENU,
	BUTTON_GAME_EXIT,

	//no button
	NO_BUTTON

} SPGUI_BUTTON_TYPE;


/*
 * structure represents a button: has windowRenderer, active image,
 * inactive image, location on the window, booleans indicate whether
 * it's visible or not and active or not, and it's enum type.
 */
typedef struct button_t {
	SDL_Renderer* windowRenderer;
	SDL_Texture* activeTexture;
	SDL_Texture* inactiveTexture;
	SDL_Rect* location;
	bool visible;
	bool active;
	SPGUI_BUTTON_TYPE type;
} Button;


/*
 * fuction creates a button/buttons set by it's params.
 */
Button* createButton(SDL_Renderer* windowRender, const char* activeImage,
		const char* inactiveImage, SDL_Rect* location, bool visible,
		bool active, SPGUI_BUTTON_TYPE type);
Button** createButtons(SDL_Renderer* windowRender, const char* activeImages[],
		const char* inactiveImages[], int xBtns[], int yBtns[], bool visible[],
		bool active[], SPGUI_BUTTON_TYPE types[], int numOfBtns);

/*
 * function destroys all memory related to button.
 */
void destroyButton(Button* src);
void destroyButtons(Button** buttons, int numOfBtns);

/*
 * function draws a button according to it's window
 * renderer and it's rectangle representing it's loaction on the window.
 */
void drawButton(Button* src);

/*
 * function creates a copy of a rectangle given.
 */
SDL_Rect* spCopyRect(SDL_Rect* src);

/*
 * The function counts how many saved games exist (0-5)
 */
int countSavedFiles();

/*
 * The function promote the saved games: each game is promoted to it's next location.
 * In addition the last saved game is removed.
 */
void promoteSlots();
/*
 * The function returns the button's type that was clicked (in the current window).
 */
SPGUI_BUTTON_TYPE getClickedButtonType(Button **buttons, int numOfButtons, SDL_Event *event, bool checkActive);



#endif /* GRAPHICS_SPCHESSGUIBUTTON_H_ */
