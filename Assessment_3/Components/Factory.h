#pragma once
#include "ObjectPool.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Controllers.h"
#include "SpriteRenderer.h"

struct entity
{
	class Factory *factory;
	ObjectPool<Transform>::iterator      trans;
	ObjectPool<RigidBody>::iterator      rb;
	ObjectPool<Controller*>::iterator    cntlr;
	ObjectPool<SpriteRenderer>::iterator sprite;
	ObjectPool<SpriteRenderer>::iterator damage_taken_sprite;
	ObjectPool<AABB>::iterator           aabb;
	//ObjectPool<lifetime>::iterator   life;
	//ObjectPool<particle>::iterator   parti;

	void onFree()
	{
		if (trans)  trans.free();
		if (rb)     rb.free();
		if (cntlr)  cntlr.free();
		if (sprite) sprite.free();
		if (aabb)   aabb.free();
		//if (life)   life.free();
		//if (parti)  parti.free();
	}
};

class Factory
{
	ObjectPool<entity>         entities;
	ObjectPool<Transform>      transforms;
	ObjectPool<RigidBody>      rigidbodies;
	ObjectPool<Controller*>    controllers;
	ObjectPool<SpriteRenderer> sprites;
	ObjectPool<SpriteRenderer> damage_taken_sprites;
	ObjectPool<AABB>		   AABBs;
	//ObjectPool<lifetime>   lifetimes;
	//ObjectPool<particle>   particles;

public:
	size_t m_size;

	Factory(int a_size) : m_size(a_size), entities(a_size), transforms(a_size), rigidbodies(a_size), controllers(a_size),
				sprites(a_size), damage_taken_sprites(a_size), AABBs(a_size) /*, lifetimes(a_size), particles(a_size) */
	{}

	ObjectPool<entity>::iterator destroy(ObjectPool<entity>::iterator &h) { h->onFree(); return h.free(); }

	ObjectPool<entity>::iterator begin() { return entities.begin(); }
	ObjectPool<entity>::iterator end()   { return entities.end(); }

	ObjectPool<entity>::iterator spawnStaticImage(unsigned sprite_id, float x, float y, float w, float h, float time = -1)
	{
		ObjectPool<entity>::iterator retVal = entities.push();
		if (!retVal) return retVal;

		retVal->factory = this;

		retVal->trans  = transforms.push();
		retVal->sprite = sprites.push();

		retVal->trans->m_position = Vec2{ x, y };
		retVal->trans->m_scale    = Vec2{ w, h };
		retVal->sprite->m_sprite  = sprite_id;

		//if (time = 0)
		//{
		//	retVal->life = lifetimes.push();
		//	retVal->life->time_lived = time;
		//}

		return retVal;
	}

	ObjectPool<entity>::iterator spawnWall(unsigned sprite_id, float xPos, float yPos,
															   float xDim, float yDim)
	{
		auto retVal = entities.push();
		if (!retVal) return retVal;
		retVal->factory = this;

		retVal->trans = transforms.push();
		retVal->sprite = sprites.push();
		retVal->aabb = AABBs.push();
		retVal->rb = rigidbodies.push();

		retVal->trans->m_position = Vec2{ xPos, yPos };
		retVal->trans->m_scale = Vec2{ xDim, yDim };

		retVal->aabb->m_pos = retVal->trans->m_position;
		retVal->aabb->m_he = Vec2(xDim*0.5, yDim*0.5);

		retVal->rb->mass = 1;

		retVal->sprite->m_sprite = sprite_id;
		retVal->sprite->m_color = WHITE;
		retVal->sprite->m_size = Vec2(xDim, yDim);

		return retVal;
	}

	ObjectPool<entity>::iterator spawnPlayer(unsigned sprite_id, unsigned damage_taken_sprite_id)
	{
		auto retVal = entities.push();
		//cout << retVal.m_idx;
		if (!retVal) return retVal;
		retVal->factory = this;

		//retVal->life   = lifetimes.push();
		retVal->trans = transforms.push();
		retVal->sprite = sprites.push();
		retVal->damage_taken_sprite = damage_taken_sprites.push();
		retVal->cntlr = controllers.push();
		retVal->aabb = AABBs.push();
		retVal->rb = rigidbodies.push();
		//retVal->parti  = particles.push();

		//retVal->life->time_lived = 0;
		//retVal->life->lifespan   = -1;

		retVal->trans->m_position = Vec2{ 400, 750 };
		retVal->trans->m_scale = Vec2{ 50, 50 };
		//retVal->parti->sDim  = vec2{ 32, 32 };
		//retVal->parti->eDim  = vec2{ 32, 32 };
		//retVal->parti->sColor.ui_color = WHITE;
		//retVal->parti->eColor.ui_color = WHITE;

		retVal->aabb->m_pos = retVal->trans->m_position;
		retVal->aabb->m_he = Vec2(50, 50);

		retVal->rb->mass = 1;
		retVal->rb->addImpulse(Vec2(-100, 0));

		retVal->sprite->m_sprite = sprite_id;
		retVal->sprite->m_color = WHITE;
		retVal->sprite->m_size = Vec2(100, 100);

		retVal->damage_taken_sprite->m_sprite = damage_taken_sprite_id;
		retVal->damage_taken_sprite->m_color = WHITE;
		retVal->damage_taken_sprite->m_size = Vec2(100, 100);

		*retVal->cntlr = new ControllerPlayer();

		return retVal;
	}

	ObjectPool<entity>::iterator spawnPlayerWeapon(unsigned sprite_id, unsigned damage_taken_sprite_id, float a_facing, Vec2 a_pos)
	{
		auto retVal = entities.push();
		if (!retVal) return retVal;
		retVal->factory = this;

		retVal->trans = transforms.push();
		retVal->sprite = sprites.push();
		retVal->damage_taken_sprite = damage_taken_sprites.push();
		retVal->cntlr = controllers.push();
		retVal->aabb = AABBs.push();
		retVal->rb = rigidbodies.push();

		retVal->trans->m_position = a_pos;
		retVal->trans->m_scale = Vec2{ 100, 20 };
		retVal->trans->m_facing = a_facing;//(int)(a_facing + 90) % 360;

		switch ((int)a_facing)
		{
		case 0:
			retVal->trans->m_position += Vec2(100, 0);
			retVal->trans->m_facing = a_facing;
			break;
		case 90:
			retVal->trans->m_position += Vec2(0, 100);
			retVal->trans->m_facing = a_facing;
			break;
		case 180:
			retVal->trans->m_position += Vec2(-100, 0);
			retVal->trans->m_facing = a_facing;
			break;
		case 270:
			retVal->trans->m_position += Vec2(0, -100);
			retVal->trans->m_facing = a_facing;
			break;
		default:
			break;
		}

		retVal->aabb->m_pos = retVal->trans->m_position;
		retVal->aabb->m_he = Vec2(50, 10);
		retVal->aabb->m_he = (rotateByDegrees(a_facing) * Vec3(retVal->trans->m_scale, 1)).xy() * 0.5;

		retVal->rb->mass = 1;

		retVal->sprite->m_sprite = sprite_id;
		retVal->sprite->m_color = WHITE;
		retVal->sprite->m_size = Vec2(100, 20);

		*retVal->cntlr = new ControllerPlayerWeapon();

		return retVal;
	}

	ObjectPool<entity>::iterator spawnCreep(unsigned sprite_id, unsigned damage_taken_sprite_id)
	{
		ObjectPool<entity>::iterator retVal = entities.push();
		if (!retVal) return retVal;
		retVal->factory = this;

		retVal->trans = transforms.push();
		retVal->sprite = sprites.push();
		retVal->cntlr = controllers.push();

		retVal->trans->m_position = Vec2{ (float)(rand() % 800), (float)(rand() % 600) };
		retVal->trans->m_scale = Vec2{ 32, 32 };

		retVal->sprite->m_sprite = sprite_id;
		retVal->sprite->m_color = WHITE;

		retVal->damage_taken_sprite->m_sprite = damage_taken_sprite_id;
		retVal->damage_taken_sprite->m_color = WHITE;

		return retVal;
	}	
	
	ObjectPool<entity>::iterator spawnBoss(unsigned sprite_id, unsigned damage_taken_sprite_id)
	{
		auto retVal = entities.push();
		//cout << retVal.m_idx;
		if (!retVal) return retVal;
		retVal->factory = this;

		//retVal->life   = lifetimes.push();
		retVal->trans = transforms.push();
		retVal->sprite = sprites.push();
		retVal->damage_taken_sprite = damage_taken_sprites.push();
		retVal->cntlr = controllers.push();
		retVal->aabb = AABBs.push();
		retVal->rb = rigidbodies.push();
		//retVal->parti  = particles.push();

		//retVal->life->time_lived = 0;
		//retVal->life->lifespan   = -1;

		retVal->trans->m_position = Vec2{ -400, 750 };
		retVal->trans->m_scale = Vec2{ 200, 150 };

		retVal->aabb->m_pos = retVal->trans->m_position;
		retVal->aabb->m_he = Vec2(100, 75);

		retVal->rb->mass = 1;
		retVal->rb->setVelocity(Vec2(100, 0));

		retVal->sprite->m_sprite = sprite_id;
		retVal->sprite->m_color = WHITE;
		retVal->sprite->m_size = Vec2(200, 150);

		retVal->damage_taken_sprite->m_sprite = damage_taken_sprite_id;
		retVal->damage_taken_sprite->m_color = WHITE;
		retVal->damage_taken_sprite->m_size = Vec2(200, 150);

		*retVal->cntlr = new ControllerBoss();

		return retVal;
	}
};