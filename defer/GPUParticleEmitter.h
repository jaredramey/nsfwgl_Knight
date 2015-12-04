#pragma once
#define GLM_SWIZZLE
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "../nsfwgl/nsfw.h"
#include "Camera.h"

class GPUParticleEmitter : public nsfw::RenderPass
{
public:
	GPUParticleEmitter(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}
	/*~GPUParticleEmitter();*/

	void prep();
	void post();
	void draw(float dT, Camera &c);
	void init(const char *nameOne, const char *nameTwo, float a_maxParticles, float lifeTimeMin, float lifeTimeMax, float velocityMin, float velocityMax, float StartSize, float EndSize, const glm::vec4 StartColor, const glm::vec4 EndColor, unsigned size);
	void update();
	void FinishUpdateShaderCreation();

	float lifeMin, lifeMax;
	float veloMin, veloMax;
	float sizeStart, sizeEnd;

	glm::vec4 startColor, endColor, pos;
	unsigned int activeBuffer;
	float lastDrawTime;

	GLuint vao[2];
	GLuint vbo[2];

	nsfw::Asset<nsfw::ASSET::SHADER> updateShader;
	nsfw::Asset<nsfw::ASSET::VAO> mesh;
	nsfw::Asset<nsfw::ASSET::SIZE> tris;
	nsfw::Asset<nsfw::ASSET::TEXTURE> color;

private:
	nsfw::ParticleVertex* particles;
	unsigned int maxParticles;
};

