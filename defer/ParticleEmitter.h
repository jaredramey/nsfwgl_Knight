#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "../nsfwgl/Assets.h"
#include <vector>
#include "../nsfwgl/Vertex.h"

class ParticleEmitter
{
	struct Particle
	{
		glm::vec4 position;
		glm::vec3 velocity;
		glm::vec4 color;
		float size;
		float lifeTime;
		float lifeSpan;

		void Update(float dT)
		{

		}
	};

	float lifeSpanMax, lifeSpanMin;
	float emissionRate = 0, emissionTime = 100;
	float velocityMax, velocityMin;
	Particle* particles;
	unsigned int firstDead;
	unsigned int maxParticles;
	glm::vec4 position;
	nsfw::Vertex* myVerts;

	float emmitTimer;
	float emmitRate;
	float startSize;
	float endSize;

	glm::vec4 startColor;
	glm::vec4 endColor;

	ParticleEmitter() : particles(nullptr), firstDead(0), maxParticles(0), position(0, 0, 0, 0){}
	~ParticleEmitter()
	{
		delete[] particles;
		delete[] myVerts;
	}

	void CreateParticles(unsigned int a_maxParticles, unsigned int a_emitRate, float a_lifeTimeMin, float a_lifeTimeMax, float a_velocityMin, float a_velocityMax, float a_startSize, float a_endSize, const glm::vec4& a_startColor, const glm::vec4& a_endColor)
	{
		emmitTimer = 0;
		emmitRate = 0;

		startColor = a_startColor;
		endColor = a_endColor;
		startSize = a_startSize;
		endSize = a_endSize;
		velocityMin = a_velocityMin;
		velocityMax = a_velocityMax;
		lifeSpanMin = a_lifeTimeMin;
		lifeSpanMax = a_lifeTimeMax;
		maxParticles = a_maxParticles;

		particles = new Particle[maxParticles];
		firstDead = 0;

		unsigned int* indexData = new unsigned int[maxParticles * 6];
		for (unsigned int i = 0; i < maxParticles; i++)
		{
			indexData[i * 6 + 0] = i * 4 + 0;
			indexData[i * 6 + 1] = i * 4 + 1;
			indexData[i * 6 + 2] = i * 4 + 2;

			indexData[i * 6 + 3] = i * 4 + 0;
			indexData[i * 6 + 4] = i * 4 + 1;
			indexData[i * 6 + 5] = i * 4 + 2;
		}

		myVerts = new nsfw::Vertex[maxParticles * 4];

		nsfw::Assets::instance().makeVAO("Particle", myVerts, 0, indexData, 4); // <-- Need to have somneone check this to make sure the call is right

		delete[] indexData;
	}

	void Emit()
	{
		if (firstDead >= maxParticles)
			return;

		Particle& particle = particles[firstDead++];

		particle.position = position;

		particle.lifeTime = 0;
		particle.lifeSpan = (rand() / (float)RAND_MAX) * (lifeSpanMax - lifeSpanMin) + lifeSpanMin;

		particle.color = startColor;
		particle.size = startSize;

		float velocity = (rand() / RAND_MAX) * (velocityMax - velocityMin) + velocityMin;
		particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
		particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
		particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
		particle.velocity = glm::normalize(particle.velocity) * velocity;
	}

	void Update(float dT, glm::mat4& cameraTransform)
	{
		emmitTimer += dT;
		while (emmitTimer > emmitRate)
		{
			Emit();
			emmitTimer -= emmitRate;
		}

		unsigned int quad = 0;

		for (unsigned int i = 0; i < firstDead; i++)
		{
			Particle* particle = &particles[i];

			particle->lifeTime += dT;
			if (particle->lifeTime >= particle->lifeSpan)
			{
				*particle = particles[firstDead - 1];
				firstDead--;
			}
			else
			{
				//move particle
				particle->position += particle->velocity * dT;
				//size particle
				particle->size = glm::mix(startSize, endSize, particle->lifeTime / particle->lifeSpan);
				//color particle
				particle->color = glm::mix(startColor, endColor, particle->lifeTime / particle->lifeSpan);

				//make a quad the correct size and color
				float halfSize = particle->size * 0.5f;

				myVerts[quad * 4 + 0].position = vec4(halfSize, halfSize, 0, 1);
				myVerts[quad * 4 + 0].normal = particle->color;
				myVerts[quad * 4 + 1].position = vec4(halfSize, halfSize, 0, 1);
				myVerts[quad * 4 + 1].normal = particle->color;
				myVerts[quad * 4 + 2].position = vec4(halfSize, halfSize, 0, 1);
				myVerts[quad * 4 + 2].normal = particle->color;
				myVerts[quad * 4 + 3].position = vec4(halfSize, halfSize, 0, 1);
				myVerts[quad * 4 + 3].normal = particle->color;

				//create billboard transform
				glm::vec3 zAxis = glm::normalize(glm::vec3(cameraTransform[3]) - glm::vec3(particle->position.x, particle->position.y, particle->position.z));
				glm::vec3 xAxis = glm::cross(glm::vec3(cameraTransform[1]), zAxis);
				glm::vec3 yAxis = glm::cross(zAxis, yAxis);

				glm::mat4 billboard(vec4(xAxis, 0), vec4(yAxis, 0), vec4(zAxis, 0), vec4(0, 0, 0, 10));

				myVerts[quad * 4 + 0].position = billboard * myVerts[quad * 4 + 0].position + particle->position;
				myVerts[quad * 4 + 1].position = billboard * myVerts[quad * 4 + 1].position + particle->position;
				myVerts[quad * 4 + 2].position = billboard * myVerts[quad * 4 + 2].position + particle->position;
				myVerts[quad * 4 + 3].position = billboard * myVerts[quad * 4 + 3].position + particle->position;

				quad++;
			}
		}
	}
};