#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Geometry.h"

class SPassPre : public nsfw::RenderPass
{
public:
	SPassPre(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}

	void prep()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glViewport(0, 0, 1024, 1024);
		glClear(GL_DEPTH_BUFFER_BIT);

		glUseProgram(*shader);
	}

	void post()
	{
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void draw(const LightD &l, const Geometry &g)
	{

		setUniform("LightMatrix", nsfw::UNIFORM::TYPE::MAT4, &(l.m_lightMatrix[0][0]));

		/*unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");*/

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, 0);
	}
};