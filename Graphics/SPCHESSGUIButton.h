#ifndef GRAPHICS_SPCHESSGUIBUTTON_H_
#define GRAPHICS_SPCHESSGUIBUTTON_H_

#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>

//button eight and width
#define BUTTON_W 150
#define BUTTON_H 50

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

} SPCHESS_BUTTON_TYPE;


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
	SPCHESS_BUTTON_TYPE type;
} Button;


/*
 * fuction creates a button/buttons set by it's params.
 */
Button* createButton(SDL_Renderer* windowRender, const char* activeImage,
		const char* inactiveImage, SDL_Rect* location, bool visible,
		bool active, SPCHESS_BUTTON_TYPE type);
Button** createButtons(SDL_Renderer* windowRender, const char* activeImages[],
		const char* inactiveImages[], int xBtns[], int yBtns[], bool visible[],
		bool active[], SPCHESS_BUTTON_TYPE types[], int numOfBtns);

/*
 * function destroys all memory related to button.
 */
void destroyButton(Button* src);
void destroyButtons(Button** btns, int numOfBtns);

/*
 * function draws a button according to it's window
 * renderer and it's rectangle representing it's loaction on the window.
 */
void drawButton(Button* src);

/*
 * function creates a copy of a rectangle given.
 */
SDL_Rect* spCopyRect(SDL_Rect* src);

#endif /* GRAPHICS_SPCHESSGUIBUTTON_H_ */
