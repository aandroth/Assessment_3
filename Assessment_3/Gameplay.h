#pragma once
#include "States.h"
#include "Factory.h"
#include "Components\ObjectPool.h"

class GamePlay
{
	unsigned credits_page_background;
	unsigned player_sprite;
	APP_STATE state;
	Factory factory;
	Camera world_camera;
	//entity wall_top, wall_left, wall_right, wall_bottom;
	ObjectPool<entity> wall_pool{4};

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
	world_camera.screenCenter = Vec2(800, 450);
	world_camera.screenZoom   = Vec2(1, 1);

	state = GAMEPLAY;
	credits_page_background = sfw::loadTextureMap("./Images/Pattern.png");
	player_sprite = sfw::loadTextureMap("./Images/Back_Button.png");
	factory.spawnPlayer(player_sprite);

	// Create Walls
	*wall_top.aabb = AABB(800, 850, 800, 25);
	*wall_top.rb = RigidBody();
	*wall_top.trans = Transform(800, 850, 1600, 50, 0);

	*wall_left.aabb = AABB(50, 450, 25, 450);
	*wall_left.rb = RigidBody();
	*wall_left.trans = Transform(50, 450, 50, 900, 0);

	*wall_right.aabb = AABB(1550, 450, 25, 450);
	*wall_right.rb = RigidBody();
	*wall_right.trans = Transform(1550, 450, 50, 900, 0);

	*wall_bottom.aabb = AABB(800, 25, 800, 25);
	*wall_bottom.rb = RigidBody();
	*wall_bottom.trans = Transform(800, 25, 1600, 50, 0);

}

void GamePlay::draw()
{
	sfw::drawTexture(credits_page_background, 800, 450, 1600, 900);
	//drawAABB(world_camera.getCameraTransform(), *wall_top.aabb);
}

void GamePlay::step(float t)
{
	auto player = factory.begin();
	//cout << "step \n";
	for (auto e = factory.begin(); e != factory.end();)
	{
		//cout << "for loop \n";
		if (e->sprite)
		{
			//cout << "draw sprite \n";
			e->sprite->draw(world_camera.getCameraTransform(), *e->trans);
			drawAABB(world_camera.getCameraTransform(), *e->aabb, GREEN);
		}
		// Controller system
		if (e->cntlr && e->trans)
		{
			(*(e->cntlr))->Update(*e->rb, *e->trans, t);
			(*e->rb).integrate(*e->trans, t);
			e->rb->debugDraw(world_camera.getCameraTransform(), *e->trans);
			(*e->rb).setVelocity(Vec2(0,0));
			e->aabb->m_pos = e->trans->m_position;
		}

		//Walls Collision System
		// Top
		CollisionData collision = aabbCollision(*wall_top.aabb, *(e->aabb));
		if (collision.resultIsCollision())
		{
			Collider colliderWall((*wall_top.aabb).verts(), 4);
			Collider colliderChar((*e->aabb).verts(), 4);
			StaticResolution((*e->trans), (*e->rb), colliderChar, (*wall_top.trans), colliderWall, 0);
			(*e->rb).integrate(*e->trans, t);
			e->aabb->m_pos = e->trans->m_position;
		}

		//*wall_left.aabb = AABB(50, 450, 25, 450);

		//*wall_right.aabb = AABB(1550, 450, 25, 450);

		//*wall_bottom.aabb = AABB(800, 25, 800, 25);
		

		++e;
	}
}

APP_STATE GamePlay::next(APP_STATE state)
{
	return state;
}

void wallCollisionResolution(entity playerOrCreep)
{
	
}