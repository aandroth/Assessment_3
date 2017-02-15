#pragma once
#include "Particles.h"
#include "Maths.h"
#include "ObjectPool.h"

#define PARTICLE_SIZE 100

class ParticleEmitter
{

public:
	ParticleEmitter() : emmissionTimer(0), particles(PARTICLE_SIZE) {}


	vec2 pos, travel_to;
	unsigned sprite;
	ObjectPool<Particle> particles;
	float emmissionTimer = 0, emitRateLo, emitRateHi;
	float angleLo, angleHi;
	float speedLo, speedHi;
	vec2 dimLoEnd, dimHiEnd;
	vec2 dimLoBeg, dimHiBeg;
	color colorLoEnd, colorHiEnd;
	color colorLoBeg, colorHiBeg;
	float lifeSpanLo, lifeSpanHi;
	float count = 1;

	Particle generateParticle()
	{
		Particle p;
		p.pos.x = lerp(pos.x, travel_to.x, count);
		p.pos.y = lerp(pos.y, travel_to.y, count);
		p.sprite = sprite;

		p.vel = randDir(angleLo, angleHi) * lerp(speedLo, speedHi, rand01());

		p.lifeSpan = lerp(lifeSpanLo, lifeSpanHi, rand01());

		p.begColor = lerp(colorLoBeg, colorHiBeg, rand01());
		p.endColor = lerp(colorLoEnd, colorHiEnd, rand01());

		p.begDim = lerp(dimLoBeg, dimLoEnd, rand01());
		p.endDim = lerp(dimHiBeg, dimHiEnd, rand01());

		p.angle = angle(p.vel);

		p.lifeTime = 0;
		return p;
	}

	void emit()
	{
		particles.push(generateParticle());
	}

	void update(float t)
	{
		if (count >= 1)
		{
			count = 0;
			pos = travel_to;
			float x = ((rand() % 800)), 
				  y = ((rand() % 600));
			travel_to = vec2{ x, y };
		}
		count += t * 0.5f;

		for (auto itr = particles.begin(); itr != particles.end(); )
		{
			itr->refresh(t);

			if (itr->isActive()) { ++itr; }
			else { itr.free(); }
		}

		emmissionTimer -= t;
		while (emmissionTimer < 0)
		{
			emit();
			emmissionTimer += lerp(emitRateLo, emitRateHi, rand01());
		}
	}

	void retargetTravelTo()
	{
		pos.x = lerp(pos.x, travel_to.x, count);
		pos.y = lerp(pos.y, travel_to.y, count);
		travel_to = vec2{sfw::getMouseX(), sfw::getMouseY()};
		count = 0;
	}
};