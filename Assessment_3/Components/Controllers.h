#pragma once
#include "Transform.h"
#include "Factory.h"

class Controller
{
protected:
	float m_speed, attackTimer, attackTimerLimit, damageTimer, damageTimerLimit;
	int m_health, m_damage;
	bool isAttacking;
public:
	bool isBeingDamaged, isWeapon, isPlayerAlly, isAlive, showDamageSprite;
	Controller() {}
	virtual int Update(RigidBody &rb, Transform &trans, unsigned sprite, float t) { cout << "Controller is updating instead of the correct controller!!!!\n"; return 0; }
	virtual void takeDamage(unsigned damage) {}
	virtual unsigned getDamage() { return m_damage; }
};

class ControllerPlayer : public Controller
{
public:
	ControllerPlayer() : Controller() {m_speed = 200, m_health = 100, m_damage = 0, showDamageSprite = false, isBeingDamaged = false, isAttacking = false, 
										isAlive = true, isWeapon = false, isPlayerAlly = true, attackTimer = 0, attackTimerLimit = 0.25f; }

	int Update(RigidBody &rb, Transform &trans, unsigned weapon_sprite, float t)
	{
		int retVal = 0;

		rb.setVelocity(Vec2(0, 0));
		//cout << trans.m_position.x;
		//cout << "ControllerPlayer Update\n";
		if (!isAttacking)
		{
			if (sfw::getKey('W'))
			{
				//cout << "detecting W\n";
				rb.setVelocity(Vec2(0, m_speed) + rb.getVelocity());
				trans.m_facing = 90;
			}
			else if (sfw::getKey('S'))
			{
				rb.setVelocity(Vec2(0, -m_speed) + rb.getVelocity());
				trans.m_facing = 270;
			}

			if (sfw::getKey('A'))
			{
				rb.setVelocity(Vec2(-m_speed, 0) + rb.getVelocity());
				trans.m_facing = 180;
			}
			else if (sfw::getKey('D'))
			{
				rb.setVelocity(Vec2(m_speed, 0) + rb.getVelocity());
				trans.m_facing = 0;
			}

			if (sfw::getKey('L'))
			{
				// Spawn sword and attack
				isAttacking = true;
				attackTimer = 0;
				retVal = 1;
			}
		}
		else
		{
			if (attackTimer >= attackTimerLimit)
			{
				isAttacking = false;
				attackTimer = 0;
			}
			else
			{
				attackTimer += t;
			}
		}
		return retVal;
	}

	void takeDamage(unsigned damage)
	{
		m_health -= damage;

		if (m_health <= 0)
			isAlive = false;
	}

	unsigned getDamage() { return m_damage; }
};

class ControllerPlayerWeapon : public Controller
{
public:
	ControllerPlayerWeapon() : Controller() { m_damage = 10, showDamageSprite = false, isAlive = true, isBeingDamaged = false, isWeapon = true, 
												isPlayerAlly = true, attackTimer = 0, attackTimerLimit = 0.25f; }

	int Update(RigidBody &rb, Transform &trans, unsigned weapon_sprite, float t)
	{
		int retVal = 0;

		if (attackTimer >= attackTimerLimit)
		{
			isAttacking = false;
			attackTimer = 0;
			retVal = 2;
		}
		else
		{
			attackTimer += t;
		}

		return retVal;
	}

	void takeDamage(unsigned damage)
	{
		m_health -= damage;

		if (m_health <= 0)
			isAlive = false;
	}

	unsigned getDamage() { return m_damage; }
};


class ControllerCreep : public Controller
{
public:
	ControllerCreep() {
		showDamageSprite = false, isAlive = true, isBeingDamaged = false, isAttacking = false, isWeapon = false, isPlayerAlly = false,
			m_damage = 0, m_speed = 400, m_health = 100, damageTimer = 0, damageTimerLimit = 0.25;
	}

	int Update(RigidBody &rb, Transform &trans, unsigned sprite, float t)
	{
		/*if (trans.m_position.x >= 1900)
		{
			rb.setVelocity(Vec2(-m_speed, 0));
		}
		else if (trans.m_position.x <= -400)
		{
			rb.setVelocity(Vec2(m_speed, 0));
		}

		if (isBeingDamaged)
		{
			damageTimer += t;
			if (damageTimer >= damageTimerLimit)
			{
				damageTimer = 0;
				isBeingDamaged = false;
				showDamageSprite = false;
			}
			else
			{
				if (damageTimer < damageTimerLimit*0.25)
					showDamageSprite = true;
				else if (damageTimer >= damageTimerLimit*0.25 && damageTimer < damageTimerLimit*0.5)
					showDamageSprite = false;
				else
					showDamageSprite = true;
			}
		}*/
		return 0;
	}

	void takeDamage(unsigned damage)
	{
		if (!isBeingDamaged)
		{
			m_health -= damage;
			isBeingDamaged = true;

			if (m_health <= 0)
			{
				isAlive = false;
			}
		}
	}

	unsigned getDamage() { return m_damage; }
};

class ControllerBoss : public Controller
{
public:
	ControllerBoss() { showDamageSprite = false, isAlive = true, isBeingDamaged = false, isAttacking = false, isWeapon = false, isPlayerAlly = false,
						m_damage = 0, m_speed = 400, m_health = 100, damageTimer = 0, damageTimerLimit = 0.25;}

	bool creep_0_active, creep_1_active, creep_2_active;
	bool creep_0_activate, creep_1_activate, creep_2_activate;

	int Update(RigidBody &rb, Transform &trans, unsigned sprite, float t)
	{
		if (trans.m_position.x >= 1900)
		{
			rb.setVelocity(Vec2(-m_speed, 0));
		}
		else if (trans.m_position.x <= -400)
		{
			rb.setVelocity(Vec2(m_speed, 0));
		}
		else if (trans.m_position.x > 400 && !creep_0_active)
		{
			creep_0_activate = true;
		}
		else if (trans.m_position.x > 700 && !creep_1_active)
		{
			creep_1_activate = true;
		}
		else if (trans.m_position.x > 1000 && !creep_2_active)
		{
			creep_2_activate = true;
		}


		if (isBeingDamaged)
		{
			damageTimer += t;
			if (damageTimer >= damageTimerLimit)
			{
				damageTimer = 0;
				isBeingDamaged = false;
				showDamageSprite = false;
			}
			else
			{
				if (damageTimer < damageTimerLimit*0.25)
					showDamageSprite = true;
				else if (damageTimer >= damageTimerLimit*0.25 && damageTimer < damageTimerLimit*0.5)
					showDamageSprite = false;
				else
					showDamageSprite = true;
			}

		}

		return 0;
	}

	void takeDamage(unsigned damage)
	{
		if (!isBeingDamaged)
		{
			m_health -= damage;
			isBeingDamaged = true;

			if (m_health <= 0)
			{
				isAlive = false;
			}
		}
	}

	unsigned getDamage() { return m_damage; }
};