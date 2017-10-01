#ifndef GRAPHICS_SPCHESSGUISETWIN_H_
#define GRAPHICS_SPCHESSGUISETWIN_H_

#include "SPGUIBasics.h"
#define NUM_OF_SET_BUTTONS 13

/*
 * enum represents set window events.
 */
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
/*
 * structure represents a set window.
 */
typedef struct spguisettingswindow_t {
	SDL_Window* settingsWindow;
	SDL_Renderer* settingsRenderer;
	Button** buttons;
	SPGame* game;

} SPGUISettingsWindow;
/*
 * standard set window functions: create destroy, draw,
 * handle events, hide and show.
 */
SPGUISettingsWindow* spSettingsWindowCreate();
void spSettingsWindowDraw(SPGUISettingsWindow *src);
void spSettingsWindowDestroy(SPGUISettingsWindow *src);
SPGUI_SETTINGS_EVENT spSettingsWindowEventHandler(SPGUISettingsWindow *src, SDL_Event *event);

#endif /* GRAPHICS_SPCHESSGUISETWIN_H_ */
