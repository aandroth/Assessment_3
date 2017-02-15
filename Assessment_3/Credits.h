#pragma once
#include "States.h"

class Credits
{
	unsigned game_stage_background;
	APP_STATE state;
public:
	void init();
	void draw();
	void step();
	APP_STATE next(APP_STATE state);
};

void Credits::init()
{
	state = CREDITS;
	game_stage_background = sfw::loadTextureMap("./Images/Credits_Page.png");
}

void Credits::draw()
{
	sfw::drawTexture(game_stage_background, 800, 450, 1600, 900);
}

void Credits::step()
{

}

APP_STATE Credits::next(APP_STATE state)
{
	return state;
}