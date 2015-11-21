#pragma once

#define GLM_SWIZZLE
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "../nsfwgl/Assets.h"

struct Particle
{
	glm::mat4 transform;
	glm::vec3 velocity;
	//glm::vec4 color;

	//float size;

	float lifeTime;	// time elapsed since birth
	float lifeSpan;	// life expectancy
};

using namespace nsfw::ASSET;

class ParticleEmitter
{
	bool emit()
	{
		// check if there are any avaiable particles to emit
		if (firstInactiveParticle >= maxParticles)
		{
			return false;
		}

		// emit the next available particle for recycling/rebirth/reincarnation
		auto& rebornParticle = particles[firstInactiveParticle++];

		// reassign particle data
		rebornParticle.transform = transform;
		rebornParticle.velocity = velocity;
		rebornParticle.lifeSpan = lifeSpan;
		rebornParticle.lifeTime = 0.0f;

		// we did it guys
		return true;
	}

public:
	ParticleEmitter(int initParticleCount,
		unsigned int initEmitRate,
		float initLifeSpan)
	{
		maxParticles = initParticleCount;

		particles = new Particle[maxParticles];
		firstInactiveParticle = 0;

		// set up timers
		emitTimer = 0.0f;
		emitRate = 1.0f / initEmitRate;

		// store emission parameters
		lifeSpan = initLifeSpan;
	}

	Particle *	particles;		// array of particles
	size_t		maxParticles;	// maximum number of particles available in pool

	int firstInactiveParticle;

	// todo@terry: make use of a transform system after tutorial is finished

	glm::mat4 transform;	// position of particle emitter
	glm::vec3 velocity;		// initial velocity of particles

	float emitTimer;		// time between each emission given in seconds	
	float emitRate;

	float lifeSpan;

	nsfw::Asset<VAO> mesh;					// mesh to draw for each particle
	nsfw::Asset<nsfw::ASSET::SIZE> tris;	// index count for mesh drawn for each particle

	nsfw::Asset<TEXTURE> color;


	void update()
	{
		float deltaTime = nsfw::Window::instance().getTime();

		emitTimer += deltaTime;
		while (emitTimer > emitRate)
		{
			emit();
			emitTimer -= emitRate;
		}

		// update particles
		for (int i = 0; i < firstInactiveParticle; ++i)
		{
			auto* curParticle = &particles[i];

			curParticle->lifeTime += deltaTime;

			// has this particle expired?
			if (curParticle->lifeTime >= curParticle->lifeSpan)
			{
				// swap last alive w/ this particle
				*curParticle = particles[firstInactiveParticle - 1];
				firstInactiveParticle--;
			}
			else
			{
				// update the particle however needed
				/*
				* Because I can't get swizzle to work
				*/
				curParticle->transform[3].x += curParticle->velocity.x * deltaTime;
				curParticle->transform[3].y += curParticle->velocity.y * deltaTime;
				curParticle->transform[3].z += curParticle->velocity.z * deltaTime;
			}

			// todo@terry: billboard the particles
		}
	}
};