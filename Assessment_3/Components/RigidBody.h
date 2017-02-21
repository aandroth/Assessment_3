#pragma once
#include "Test.h"
#include "Transform.h"
#include "sfwdraw.h"
#include "Camera.h"
#include <iostream>
using std::cout;

class RigidBody
{
public:
	Vec2 force, impulse;
	Vec2 velocity, accelleration;
	float angularVelocity = 0, angularAccelleration, torque;
	float mass, drag;
	bool isStatic;

	RigidBody();

	void integrate(Transform &trans, const float deltaTime);

	void addForce(const Vec2 & a_force);
	void addImpulse(const Vec2 & an_impulse);
	void addTorque(float torque);

	void setAccelleration(Vec2);
	Vec2 getAccelleration() const;

	void setVelocity(Vec2);
	Vec2 getVelocity() const;

	void debugDraw(const Mat3 & T, const Transform);
};

