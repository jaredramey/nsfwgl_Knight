#pragma once
#define GLM_SWIZZLE
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "../nsfwgl/nsfw.h"

class GPUParticleEmitter
{
public:
	GPUParticleEmitter();
	~GPUParticleEmitter();

	void init();
	void emit();
	void update();
};

