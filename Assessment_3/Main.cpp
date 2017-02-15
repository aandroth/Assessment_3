#include "Collision.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Camera.h"
#include "sfwdraw.h"
#include "Shapes.h"
#include <cmath>
#include <string>
#include <time.h>
#include "Mat3.h"
#include <vector>
#include "Collider.h"
#include "Menu.h"
#include "SplashPage.h"
#include "Gameplay.h"
#include "Credits.h"
#include <iostream>
using std::cout;
unsigned font;

int main()
{
	SplashPage *splash_page_ptr    = nullptr;
	Menu       *menu_page_ptr      = nullptr;
	GamePlay   *gameplay_level_ptr = nullptr;
	Credits    *credits_page_ptr   = nullptr;

	// Create the window
	sfw::initContext(1600, 900, "Player vs Object Pool");

	float b = 0.5, m = 0.5, e = 1.0f, tightness = 0.1;
	bool onV1 = true;
	font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);
	float grav = 0;
	int timeStep = 0;
	Transform cameraTransform;

	APP_STATE state = ENTER_SPLASH;
	Menu menu;
	menu.init();
	float t = sfw::getDeltaTime();

	while (sfw::stepContext())
	{
		switch (state)
		{
		case ENTER_SPLASH:
			splash_page_ptr = new SplashPage();
			splash_page_ptr->init(font);
			state = SPLASH;
			break;
		case SPLASH:
			state = splash_page_ptr->next(state);
			splash_page_ptr->step(t);
			splash_page_ptr->draw();
			break;
		case ENTER_MENU:
			menu_page_ptr = new Menu();
			menu_page_ptr->init();
			state = MENU;
			break;
		case MENU:
			menu_page_ptr->step();
			menu.draw();
			if (menu_page_ptr->next() == STAY) {}
			else if (menu_page_ptr->next() == PLAY_GAME)    { state = ENTER_GAMEPLAY; }
			else if (menu_page_ptr->next() == SHOW_CREDITS) { state = ENTER_CREDITS; }
			else if (menu_page_ptr->next() == EXIT_MENU)    { state = EXIT; }
			else { return 0; }
			break;
		case ENTER_GAMEPLAY:
			gameplay_level_ptr = new GamePlay();
			gameplay_level_ptr->init();
			state = GAMEPLAY;
			break;
		case GAMEPLAY:
			state = gameplay_level_ptr->next(state);
			gameplay_level_ptr->step();
			gameplay_level_ptr->draw();
			break;
		case ENTER_CREDITS:
			credits_page_ptr = new Credits;
			credits_page_ptr->init();
			state = CREDITS;
			break;
		case CREDITS:
			state = credits_page_ptr->next(state);
			credits_page_ptr->step();
			credits_page_ptr->draw();
			break;
		case EXIT:
			return 0;
		}
	}
	sfw::termContext();

	return 0;
}