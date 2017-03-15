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
	Controller * parentCntlr;
	int m_creep_count;
	bool isBeingDamaged, isWeapon, isPlayerAlly, isAlive, showDamageSprite;
	//bool creep_0_active, creep_1_active, creep_2_active;
	//bool creep_0_activate, creep_1_activate, creep_2_activate;
	bool creeps_activate, creeps_active;

	Controller() {}
	virtual int Update(unsigned sprite, float t) { cout << "Controller is updating instead of the correct controller!!!!\n"; return 0; }
	virtual void takeDamage(unsigned damage) {}
	virtual unsigned getDamage() { return m_damage; }
	virtual Vec2 getSpawnLocation(Vec2 boss_pos, Vec2 player_pos) { return Vec2(0, 0); }
	virtual void TravelTo(RigidBody &rb, Transform &trans, Vec2 dest);
};

class ControllerPlayer : public Controller
{
public:
	ControllerPlayer() : Controller() {m_speed = 200, m_health = 100, m_damage = 0, showDamageSprite = false, isBeingDamaged = false, isAttacking = false, 
										isAlive = true, isWeapon = false, isPlayerAlly = true, attackTimer = 0, attackTimerLimit = 0.25f; }

	int Update(unsigned weapon_sprite, float t)
	{
		int retVal = 0;

		//cout << trans.m_position.x;
		//cout << "ControllerPlayer Update\n";
		if (!isAttacking)
		{
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

	void TravelTo(RigidBody &rb, Transform &trans, Vec2 unused_v)
	{
		rb.setVelocity(Vec2(0, 0));

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
		}
	}
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

	void TravelTo(RigidBody &rb, Transform &trans, Vec2 unused_v) {}
};


class ControllerCreep : public Controller
{
public:
	ControllerCreep() { showDamageSprite = false, isAlive = true, isBeingDamaged = false, isAttacking = false, isWeapon = true, isPlayerAlly = false,
			m_damage = 10, m_speed = 400, m_health = 20, damageTimer = 0, damageTimerLimit = 0.3; }

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
*/
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
				parentCntlr->m_creep_count -= 1;
			}
		}
	}

	unsigned getDamage() { return m_damage; }

	void TravelTo(RigidBody &rb, Transform &trans, Vec2 dest)
	{
		rb.setVelocity(Vec2(0, 0));

		if (!isAttacking)
		{
			Vec2 dir = dest - trans.m_position;
			dir = normal(dir);

			rb.setVelocity(dir*m_speed + rb.getVelocity());
		}
	}
};

class ControllerBoss : public Controller
{
private:
	bool collectingDead, exitLeft, exitRight;
	float distanceAlongCurve;
	Vec2 startPoint, middlePoint, endPoint;

public:
	ControllerBoss() { showDamageSprite = false, isAlive = true, isBeingDamaged = false, isAttacking = false, isWeapon = false, isPlayerAlly = false,
						m_damage = 0, m_speed = 400, m_health = 100, damageTimer = 0, damageTimerLimit = 0.3,
						creeps_active = false, creeps_activate = false;}

	int Update(RigidBody &rb, Transform &trans, unsigned sprite, float t)
	{
		//if (trans.m_position.x >= 1700)
		//{
		//	rb.setVelocity(Vec2(-m_speed, 0));
		//}
		//else if (trans.m_position.x <= 000)
		//{
		//	rb.setVelocity(Vec2(m_speed, 0));
		//}
		//else if (trans.m_position.x > 400 && !creep_0_active)
		//{
		//	creep_0_activate = true;
		//}
		//else if (trans.m_position.x > 700 && !creep_1_active)
		//{
		//	creep_1_activate = true;
		//}
		//else if (trans.m_position.x > 1000 && !creep_2_active)
		//{
		//	creep_2_activate = true;
		//}

		distanceAlongCurve += m_speed * t;
		if (distanceAlongCurve > 1)
		{
			distanceAlongCurve = 0;
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
				if (damageTimer < damageTimerLimit*0.5)
					showDamageSprite = true;
				else if (damageTimer >= damageTimerLimit*0.5 && damageTimer < damageTimerLimit*0.75)
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

	Vec2 getSpawnLocation(Vec2 boss_pos, Vec2 player_pos)
	{
		bool top = true, left = true, right = true, bottom = true;
		Vec2 place_creep_pos;
		// Consider top
		if (player_pos.y < boss_pos.y)
			bottom = false;
		else
			top = false;
		if (player_pos.x < boss_pos.x)
			left = false;
		else
			right = false;

		if (boss_pos.y < 500)
			bottom = false;
		else
			top = false;
		if (boss_pos.x < 900)
			left = false;
		else
			right = false;

		if (!top && !bottom && !left && !right)
		{
			top = bottom = left = right = true;

			if (boss_pos.y < 500)
				bottom = false;
			else
				top = false;
			if (boss_pos.x < 900)
				left = false;
			else
				right = false;
		}

		if (top)
			return boss_pos + Vec2(0, 100);
		if (bottom)
			return boss_pos + Vec2(0, -100);
		if (left)
			return boss_pos + Vec2(-200, 0);
		if (right)
			return boss_pos + Vec2(200, 0);
	}

	void TravelTo(RigidBody &rb, Transform &trans, Vec2 dest)
	{
		trans.m_position = quadBezier(startPoint, endPoint, middlePoint, distanceAlongCurve);
	}

	void GetNewBezier(Vec2 end)
	{
		startPoint  = endPoint;
		endPoint    = end;
		middlePoint = Vec2(rand() % abs((int)(startPoint.y - endPoint.y)), rand() % abs((int)(startPoint.x - endPoint.x)));
	}
};