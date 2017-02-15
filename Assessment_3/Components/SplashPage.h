#pragma once

#include "States.h"
#include <time.h>
#include "sfwdraw.h"

class SplashPage
{
	int splashFont;
	float currentTime;
public:
	void init(unsigned font);
	void play();
	void draw();
	void step(float t);
	APP_STATE next(APP_STATE);
};

void SplashPage::init(unsigned font)
{
	splashFont = font;
	currentTime = .5f;
}

void SplashPage::play()
{
	currentTime = 2.f;
}

void SplashPage::draw()
{
	sfw::drawString(splashFont, "Aquin's Great Games!", 200, 600, 50, 50, 0, ' ');
	sfw::drawLine(0, 300, 500 * (currentTime / 3), 300, 0xffffff);
}

void SplashPage::step(float t)
{
	currentTime -= sfw::getDeltaTime();
}

APP_STATE SplashPage::next(APP_STATE state)
{
	if (currentTime <= 0)
	{
		return ENTER_MENU; // GAMEPLAY;
	}
	else
	{
		return state;
	}
}
#pragma once
