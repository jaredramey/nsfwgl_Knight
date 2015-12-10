#pragma once
#include "../nsfwgl/nsfw.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"


struct GPUParticle
{
	GPUParticle() : lifetime(1), lifespan(0) {}

	glm::vec3 position;
	glm::vec3 velocity;
	float lifetime;
	float lifespan;
};

class GPUParticleEmitter
{
public:
	GPUParticleEmitter();
	virtual ~GPUParticleEmitter();

	void Init(unsigned int a_maxParticles,
		float a_lifespanMin, float a_lifespanMax,
		float a_velocityMin, float a_velocityMax,
		float a_startSize, float a_endSize,
		const glm::vec4& a_startColor,
		const glm::vec4& a_endColor);

	void Draw(float time, const glm::mat4& a_cameraTransform, const glm::mat4& a_projectionView);

	void SetPosition(glm::vec3 a_position) { mPosition = a_position; }
	glm::vec3 GetPosition() { return mPosition; }

protected:

	void CreateBuffers();
	void CreateUpdateShader();
	void CreateDrawShader();
	unsigned int LoadShader(unsigned int type, const char* path);

	GPUParticle* mParticles;

	unsigned int mMaxParticles;

	glm::vec3 mPosition;

	float mLifespanMin;
	float mLifespanMax;

	float mVelocityMin;
	float mVelocityMax;

	float mStartSize;
	float mEndSize;

	glm::vec4 mStartColor;
	glm::vec4 mEndColor;

	unsigned int mActiveBuffer;
	unsigned int mVAO[2];
	unsigned int mVBO[2];

	unsigned int mDrawShader;
	unsigned int mUpdateShader;

	float mLastDrawTime;
};