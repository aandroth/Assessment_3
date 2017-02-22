#pragma once
#include "States.h"
#include "Factory.h"
#include "Components\ObjectPool.h"

class GamePlay
{
	unsigned credits_page_background;
	unsigned player_sprite;
	APP_STATE state;
	Factory *factory, *wall_factory;
	Camera world_camera;
public:

	void FillObjectPool();
	void init();
	void draw();
	void step(float t);
	APP_STATE next(APP_STATE state);
};

void GamePlay::FillObjectPool()
{
	factory = new Factory(20);
	wall_factory = new Factory(4);
	// Create the player
}

void GamePlay::init()
{
	world_camera.screenCenter = Vec2(0, 0);
	world_camera.screenZoom   = Vec2(1, 1);

	state = GAMEPLAY;
	credits_page_background = sfw::loadTextureMap("./Images/Pattern.png");
	player_sprite = sfw::loadTextureMap("./Images/Back_Button.png");
	factory->spawnPlayer(player_sprite);

	// Create Walls
	wall_factory->spawnWall(player_sprite, 800, 850, 1600, 50);
	//wall_factory->spawnWall(player_sprite, 50, 450, 50, 900);
	//wall_factory->spawnWall(player_sprite, 1550, 450, 50, 900);
	//wall_factory->spawnWall(player_sprite, 800, 25, 1600, 50);
}

void GamePlay::draw()
{
	sfw::drawTexture(credits_page_background, 800, 450, 1600, 900);
	//drawAABB(world_camera.getCameraTransform(), *wall_top.aabb);
	drawAABB(world_camera.getCameraTransform(), *wall_factory->begin()->aabb, RED);
	drawAABB(world_camera.getCameraTransform(), *factory->begin()->aabb, RED);
}

void GamePlay::step(float t)
{
	auto player = factory->begin();
	//cout << "step \n";
	for (auto e = factory->begin(); e != factory->end();)
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

			//Walls Collision System
			for (auto w = wall_factory->begin(); w != wall_factory->end();)
			{	
				CollisionData collision = aabbCollision(*e->aabb, *(w->aabb));
				if (!collision.resultIsCollision())
				{
					 // Do nothing
				}
				else
				{
					//cout << "Do something\n";
					Collider colliderWall((*w->aabb).verts(), 4);
					Collider colliderChar((*e->aabb).verts(), 4);
					Vec2 *v = (*w->aabb).verts();
					cout << "v.x: " << v[0].x << ", v.y: " << v[0].y << "\n";
					cout << "v[1].x: " << v[1].x << ", v[1].y: " << v[1].y << "\n";
					cout << "v[2].x: " << v[2].x << ", v[2].y: " << v[2].y << "\n";
					cout << "v[3].x: " << v[3].x << ", v[3].y: " << v[3].y << "\n";
					StaticResolution((*e->trans), (*e->rb), colliderChar, (*w->trans), colliderWall, 0);
					//(*e->rb).integrate(*e->trans, t);
					//e->aabb->m_pos = e->trans->m_position;
				}
				w->sprite->draw(world_camera.getCameraTransform(), *w->trans);
				++w;
			}

			(*e->rb).integrate(*e->trans, t);
			e->rb->debugDraw(world_camera.getCameraTransform(), *e->trans);
			(*e->rb).setVelocity(Vec2(0,0));
			e->aabb->m_pos = e->trans->m_position;
		}
		
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