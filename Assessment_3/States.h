#pragma once

enum APP_STATE
{
	ENTER_SPLASH,
	SPLASH,
	ENTER_MENU,
	MENU,
	ENTER_GAMEPLAY,
	GAMEPLAY,
	ENTER_CREDITS,
	CREDITS,
	GAMEOVER,
	EXIT
};

enum MENU_STATE
{
	STAY,
	PLAY_GAME,
	SHOW_CREDITS,
	EXIT_MENU
};