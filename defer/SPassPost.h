#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class SPassPost : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> shadow;
public:
	SPassPost(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}

	void prep()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(*shader);
	}

	void post()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}

	void draw(const LightD &l, Camera &c)
	{
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
		setUniform("View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetView()));

		glm::mat4 textureSpaceOffset(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f);

		glm::mat4 lightMatrix = textureSpaceOffset * l.m_lightMatrix;

		setUniform("LightMatrix", nsfw::UNIFORM::TYPE::MAT4, &lightMatrix[0][0]);
		setUniform("lightDir", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.m_lightMatrix));
		setUniform("shadowMap", nsfw::UNIFORM::TYPE::TEX2, shadow, 0);

		unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
	}
};