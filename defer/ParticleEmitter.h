#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "../nsfwgl/Assets.h"
#include <vector>

class ParticleEmitter
{
	float emissionRate;
	float emissionTime;

	struct Particle
	{
		glm::vec4 position;
		glm::vec3 velocity;
		glm::vec3 color;
		float size;
		float lifeTime;
		float lifeSpan;

		void Update(float dT)
		{

		}
	};

	std::vector<Particle> particles;

	void CreateParticles(float amount)
	{
		for (int i = 0; i < amount; i++)
		{
			Particle newParticle;
			newParticle.position = glm::vec4(1, 2, 2, 1);
			newParticle.velocity = glm::vec3(1);
			newParticle.color	 = glm::vec3(1.f, 1.f, 0.0f);
			newParticle.size	 = 5.f;
			newParticle.lifeTime = 0.0f;
			newParticle.lifeSpan = 50.f;

			particles.push_back(newParticle);
		}
	}

	void Emit(std::vector<Particle> particleList)
	{
		for (int i = 0; i < particleList.size(); i++)
		{
			if (particleList[i].lifeTime <= 0)
			{
				return;
			}


		}
	}

	void Update(float dT)
	{

	}
};