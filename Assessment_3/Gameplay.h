#pragma once
#include "States.h"
#include "Factory.h"
#include "Components\ObjectPool.h"

class GamePlay
{
	unsigned credits_page_background;
	unsigned player_sprite, sword_sprite, boss_sprite, creep_sprite, damaged_sprite, blackout_sprite;
	APP_STATE state;
	Factory *factory, *wall_factory;
	Camera world_camera;
	bool gameIsOver;
	ObjectPool<entity>::iterator player_ptr, boss_ptr;
public:

	void FillObjectPool();
	void init();
	void draw();
	void step(float t);
	APP_STATE next();
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
	gameIsOver = false;

	credits_page_background = sfw::loadTextureMap("./Images/Pattern.png");
	player_sprite = sfw::loadTextureMap("./Images/Back_Button.png");
	boss_sprite = sfw::loadTextureMap("./Images/Boss_Box.png");
	creep_sprite = sfw::loadTextureMap("./Images/Creep_Box.png");
	sword_sprite = sfw::loadTextureMap("./Images/Sword_Box.png");
	damaged_sprite = sfw::loadTextureMap("./Images/Damaged_Box.png");
	blackout_sprite = sfw::loadTextureMap("./Images/Blackout_Box.png");

	boss_ptr   = factory->spawnBoss(boss_sprite, damaged_sprite);
	player_ptr = factory->spawnPlayer(player_sprite, damaged_sprite);

	// Create Walls
	wall_factory->spawnWall(player_sprite, 800, 850, 1600, 50);
	wall_factory->spawnWall(player_sprite, 50, 450, 50, 900);
	wall_factory->spawnWall(player_sprite, 1550, 450, 50, 900);
	wall_factory->spawnWall(player_sprite, 800, 25, 1600, 50);
}

void GamePlay::draw()
{
	sfw::drawTexture(credits_page_background, 800, 450, 1600, 900);
	for (auto wall_ptr = wall_factory->begin(); wall_ptr != wall_factory->end(); ++wall_ptr)
	{
		drawAABB(world_camera.getCameraTransform(), *wall_ptr->aabb, RED);
	}
	for (auto e = factory->begin(); e != factory->end(); ++e)
	{
		drawAABB(world_camera.getCameraTransform(), *e->aabb, RED);
	}
}

void GamePlay::step(float t)
{
	//cout << "step \n";
	for (auto e = factory->begin(); e != factory->end();)
	{
		if (e == factory->begin() && (*e->cntlr)->isAlive)
		{
			if ((*e->cntlr)->creep_0_activate)
			{
				factory->spawnCreep(creep_sprite, damaged_sprite, (*e->cntlr)->getSpawnLocation((*e->trans).m_position, (*player_ptr->trans).m_position));
				(*e->cntlr)->creep_0_activate = false;
				(*e->cntlr)->creep_0_active = true;
			}
		}
		else if(e == factory->begin() && !(*e->cntlr)->isAlive)
		{
			gameIsOver = true;
			return;
		}
		//if (e != factory->begin() && !(*e->cntlr)->isPlayerAlly && !(*e->cntlr)->isAlive)
		//{
		//	(*boss_ptr->cntlr)->creep_0_activate = true;
		//}

		if ((*e->cntlr)->isAlive)
		{
			//cout << "for loop \n";
			if (e->sprite)
			{
				if ((*(e->cntlr))->showDamageSprite)
					e->damage_taken_sprite->draw(world_camera.getCameraTransform(), *e->trans);
				else
					e->sprite->draw(world_camera.getCameraTransform(), *e->trans);
			}
			// Controller system
			if (e->cntlr && e->trans)
			{
				int attackCode = (*(e->cntlr))->Update(*e->rb, *e->trans, 1, t);
				if (attackCode == 0) {/* Do nothing*/ }
				else if (attackCode == 1)
				{
					float facing = e->trans->m_facing;
					Vec2 pos = e->trans->m_position;
					factory->spawnPlayerWeapon(sword_sprite, sword_sprite, facing, pos);
				}
				else if (attackCode == 2) 
				{
					factory->destroy(e); 
					continue; 
				}

				//Walls Collision System
				if (e != factory->begin() && !((*e->cntlr)->isWeapon))
				{
					for (auto w = wall_factory->begin(); w != wall_factory->end();)
					{
						CollisionData collision = aabbCollision(*e->aabb, *(w->aabb));
						if (!collision.resultIsCollision())
						{
							// Do nothing
						}
						else
						{
							Collider colliderWall((*w->aabb).verts(), 4);
							Collider colliderChar((*e->aabb).verts(), 4);
							StaticResolution((*e->trans), (*e->rb), colliderChar, (*w->trans), colliderWall, 0);
							e->aabb->m_pos = e->trans->m_position;
						}
						++w;
					}
				}

				auto f = e;
				++f;
				for (; f != factory->end(); ++f)
				{
					if ((*f->cntlr)->isAlive)
					{
						CollisionData collision = aabbCollision(*e->aabb, *(f->aabb));
						if (collision.resultIsCollision())
						{
							if (((*e->cntlr)->isWeapon) && !((*f->cntlr)->isPlayerAlly) && ((*e->cntlr)->isPlayerAlly != (*f->cntlr)->isPlayerAlly))
							{
								(*f->cntlr)->takeDamage((*e->cntlr)->getDamage());
							}
							else if (((*f->cntlr)->isWeapon) && !((*e->cntlr)->isPlayerAlly) && ((*e->cntlr)->isPlayerAlly != (*f->cntlr)->isPlayerAlly))
							{
								(*e->cntlr)->takeDamage((*f->cntlr)->getDamage());
							}
							if (e != factory->begin() && !(*f->cntlr)->isPlayerAlly)
							{
								Collider colliderWall((*f->aabb).verts(), 4);
								Collider colliderChar((*e->aabb).verts(), 4);
								StaticResolution((*e->trans), (*e->rb), colliderChar, (*f->trans), colliderWall, 10);
								e->aabb->m_pos = e->trans->m_position;
							}
						}
					}
				}
				e->aabb->m_pos = e->trans->m_position;
				(*e->rb).integrate(*e->trans, t);
				e->rb->debugDraw(world_camera.getCameraTransform(), *e->trans);
			}
			++e;
		}
		else
		{
			factory->destroy(e);
			(*boss_ptr->cntlr)->creep_0_activate = true;
			(*boss_ptr->cntlr)->creep_0_active = false;
			continue;
		}
	}

	if ((*(boss_ptr->cntlr))->showDamageSprite)
		boss_ptr->damage_taken_sprite->draw(world_camera.getCameraTransform(), *boss_ptr->trans);
	else
		boss_ptr->sprite->draw(world_camera.getCameraTransform(), *boss_ptr->trans);
}

APP_STATE GamePlay::next()
{
	if (!gameIsOver)
	{
		return GAMEPLAY;
	}
	else
	{
		return ENTER_CREDITS;
	}
}