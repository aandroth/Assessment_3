#pragma once
#include "Transform.h"

class Controller
{
protected:
	float m_speed;
public:
	Controller() {}
	virtual void Update(RigidBody &rb, Transform &trans, float t) { cout << "Controller is udating instead of the correct controller!!!!\n"; }
};

class ControllerPlayer : public Controller
{
public:
	ControllerPlayer() : Controller() { m_speed = 200; }

	void Update(RigidBody &rb, Transform &trans, float t)
	{
		//cout << "ControllerPlayer Update\n";
		if (sfw::getKey('W'))
		{
			//cout << "detecting W\n";
			//trans.m_position.y += m_speed * t;
			rb.setVelocity(Vec2(0, m_speed) + rb.getVelocity());

		}
		else if (sfw::getKey('S'))
		{
			//trans.m_position.y -= m_speed * t;
			rb.setVelocity(Vec2(0, -m_speed) + rb.getVelocity());
		}

		if (sfw::getKey('A'))
		{
			//trans.m_position.x -= m_speed * t;
			rb.setVelocity(Vec2(-m_speed, 0) + rb.getVelocity());
		}
		else if (sfw::getKey('D'))
		{
			//trans.m_position.x += m_speed * t;
			rb.setVelocity(Vec2(m_speed, 0) + rb.getVelocity());
		}
	}
};

class ControllerCreep : public Controller
{
public:
	ControllerCreep() { m_speed = 10; }
	void Update(RigidBody &rb, Transform &trans, float t)
	{

	}
};

class ControllerBoss : public Controller
{
public:
	ControllerBoss() { m_speed = 10; }

	void Update(RigidBody &rb, Transform &trans, float t)
	{

	}
};

