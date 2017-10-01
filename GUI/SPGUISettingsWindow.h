#ifndef GRAPHICS_SPCHESSGUISETWIN_H_
#define GRAPHICS_SPCHESSGUISETWIN_H_

#include "SPGUIBasics.h"
#define NUM_OF_SET_BUTTONS 13

// enum which represents events in settings window
typedef enum {
	SPGUI_SETTINGS_GAME_MODE,
	SPGUI_SETTINGS_DIFF,
	SPGUI_SETTINGS_COLOR,
	SPGUI_SETTINGS_START,
	SPGUI_SETTINGS_BACK,
	SPGUI_SETTINGS_INVALID_ARGUMENT,
	SPGUI_SETTINGS_NONE,
	SPGUI_SETTINGS_QUIT
} SPGUI_SETTINGS_EVENT;

// Settings window struct
typedef struct spguisettingswindow_t {
	SDL_Window* settingsWindow;
	SDL_Renderer* settingsRenderer;
	Button** buttons;
	SPGame* game;

} SPGUISettingsWindow;

/**
 * Creates a new settings window
 *
 * @return
 * NULL if either a memory allocation failure.
 * Otherwise, a new settings window is returned.
 */
SPGUISettingsWindow* spSettingsWindowCreate();

/**
 * Draws the given settings window
 */
void spSettingsWindowDraw(SPGUISettingsWindow *src);

/**
 * free the memory located by the given settings window
 */
void spSettingsWindowDestroy(SPGUISettingsWindow *src);

/**
 * The function handles an event that accured in the
 * settings window, and returns the event that happened
 * for further handling (by the controller)
 *
 * @return
 * SPGUI_SETTINGS_GAME_MODE - if new game mode have been chosen
	SPGUI_SETTINGS_DIFF - if new difficulty have been chosen
	SPGUI_SETTINGS_COLOR - if new player's color have been chosen
	SPGUI_SETTINGS_START - if the start button have been pressed
	SPGUI_SETTINGS_BACK - if the back button have been pressed
	SPGUI_SETTINGS_INVALID_ARGUMENT - if an invalid argument have been given to the handler
	SPGUI_SETTINGS_QUIT - if a quit have been requested
 	SPGUI_SETTINGS_NONE - if none of the above happened
 */
SPGUI_SETTINGS_EVENT spSettingsWindowEventHandler(SPGUISettingsWindow *src, SDL_Event *event);

#endif /* GRAPHICS_SPCHESSGUISETWIN_H_ */
