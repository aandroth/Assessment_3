#pragma once
#include "States.h"

class GamePlay
{
	unsigned credits_page_background;
	APP_STATE state;

public:
	void init();
	void draw();
	void step();
	APP_STATE next(APP_STATE state);
};

void FillObjectPool()
{
	// Create the room

	// Place the player
}

void TakeInputFromPlayer()
{

}

void GamePlay::init()
{
	state = GAMEPLAY;
	credits_page_background = sfw::loadTextureMap("./Images/Game_Stage.png");
}

void GamePlay::draw()
{
	sfw::drawTexture(credits_page_background, 800, 450, 1600, 900);
}

void GamePlay::step()
{

}

APP_STATE GamePlay::next(APP_STATE state)
{
	return state;
}
