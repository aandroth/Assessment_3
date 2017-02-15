#pragma once
#include "States.h"
#include "Components\Factory.h"

class GamePlay
{
	unsigned credits_page_background;
	unsigned player_sprite;
	APP_STATE state;
	Factory factory;

	void FillObjectPool();

public:
	void init();
	void draw();
	void step(float t);
	APP_STATE next(APP_STATE state);
};

void GamePlay::FillObjectPool()
{
	// Create the player
	factory.spawnPlayer(credits_page_background);
}

void GamePlay::init()
{
	state = GAMEPLAY;
	credits_page_background = sfw::loadTextureMap("./Images/Game_Stage.png");
	player_sprite = sfw::loadTextureMap("./Images/Back_Button.png");
	factory.spawnPlayer(player_sprite);
}

void GamePlay::draw()
{
	sfw::drawTexture(credits_page_background, 800, 450, 1600, 900);
}

void GamePlay::step(float t)
{
	//cout << "step \n";
	for (auto e = factory.begin(); e != factory.end();)
	{
		//cout << "for loop \n";
		if (e->sprite)
		{
			//cout << "draw sprite \n";
			e->sprite->draw(*e->trans);
		}

		// Controller system
		if (e->cntlr && e->trans)
		{
			(*(e->cntlr))->Update(*e->trans, t);
		}

		++e;
	}
}

APP_STATE GamePlay::next(APP_STATE state)
{
	return state;
}
