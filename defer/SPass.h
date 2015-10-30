#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"

class SPass : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> position;
	glm::vec3 m_lightDirection = glm::vec3(0);
	glm::mat4 lightProjection = glm::mat4(0), lightView = glm::mat4(0), m_lightMatrix = glm::mat4(0);
public:
	SPass(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}

	void prep()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(*shader);
	}

	void post()
	{
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void draw(const LightD &l)
	{
		m_lightDirection = glm::normalize(l.direction);
		lightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
		m_lightMatrix = glm::lookAt(m_lightDirection, glm::vec3(0), glm::vec3(0,1,0));

		setUniform("LightMatrix", nsfw::UNIFORM::TYPE::MAT4, &(m_lightMatrix[0][0]));
	}
};