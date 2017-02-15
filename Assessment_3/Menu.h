#pragma once

#include <time.h>
#include "sfwdraw.h"
#include "States.h"
#include "Button.h"
#include <iostream>
using std::cout;

class Menu
{
	Button playButton, creditsButton, exitButton;
	MENU_STATE state = STAY;
	int splashFont;
	float currentTime;

	unsigned play_button    = sfw::loadTextureMap("./Images/PLAY_Button.png");
	unsigned credits_button = sfw::loadTextureMap("./Images/PLAY_Button.png");
	unsigned exit_button    = sfw::loadTextureMap("./Images/EXIT_Button.png");
	unsigned mouse          = sfw::loadTextureMap("./Images/PLAY_Button.png", 10, 10);

	void buttonPressed_Play();
	void buttonPressed_Credits();
	void buttonPressed_Exit();

public:
	void init();
	void play();
	void draw();
	void step();
	MENU_STATE next();
};

void Menu::buttonPressed_Play()
{
	state = PLAY_GAME;
}

void Menu::buttonPressed_Credits()
{
	state = SHOW_CREDITS;
}

void Menu::buttonPressed_Exit()
{
	state = EXIT_MENU;
}

void Menu::init()
{
	// Play Button
	playButton.mouseDepressedOverButton = false;
	playButton.m_pos = Vec2(450, 600);
	playButton.m_dim = Vec2(100, 50);
	playButton.m_text = "EXIT";
	playButton.m_texture = sfw::loadTextureMap("./Images/LAZERZ_Button.png");

	// Credits Button
	creditsButton.mouseDepressedOverButton = false;
	creditsButton.m_pos = Vec2(450, 450);
	creditsButton.m_dim = Vec2(100, 50);
	creditsButton.m_text = "EXIT";
	creditsButton.m_texture = sfw::loadTextureMap("./Images/HULL_Button.png");
	//nGonButton.m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

	// EXIT Button
	exitButton.mouseDepressedOverButton = false;
	exitButton.m_pos = Vec2(450, 300);
	exitButton.m_dim = Vec2(100, 50);
	exitButton.m_text = "EXIT";
	exitButton.m_texture = sfw::loadTextureMap("./Images/EXIT_Button.png");
	//exitButton.m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

	state = STAY;
}

void Menu::play()
{
	currentTime = 2.f;
}

void Menu::draw()
{
	playButton.draw();
	creditsButton.draw();
	exitButton.draw();

	sfw::drawTexture(exit_button, sfw::getMouseX(), sfw::getMouseY(), 10, 10);
}

void Menu::step()
{
	if (playButton.mouseUp())
	{
		state = PLAY_GAME;
	}
	playButton.mouseDown();

	if (creditsButton.mouseUp())
	{
		state = SHOW_CREDITS;
	}
	creditsButton.mouseDown();

	if (exitButton.mouseUp())
	{
		state = EXIT_MENU;
	}
	exitButton.mouseDown();

}

MENU_STATE Menu::next()
{
	return state;
}
#pragma once
