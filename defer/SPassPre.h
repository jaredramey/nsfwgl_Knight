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
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);

		glUseProgram(*shader);
	}

	void post()
	{
		glViewport(0, 0, nsfw::Window::instance().getWidth(), nsfw::Window::instance().getHeight());
		glDisable(GL_DEPTH_TEST);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void draw(const LightD &l, const Geometry &g)
	{

		
		setUniform("LightView", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(l.getView()));
		setUniform("LightProjection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(l.getProjection()));

		setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, 0);
	}
};