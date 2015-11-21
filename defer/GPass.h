#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"
#include "ParticleEmitter.h"

class GPass : public nsfw::RenderPass
{


public:	
	void prep()
	{ 
		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc..."); 
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(*shader);
	
	}
	void post() 
	{ 
		//TODO_D("Unset any gl settings");
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
		glBindVertexArray(0);
	}

	GPass(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}

	/*
	 * draw call for particles
	*/
	void draw(Camera &c, const ParticleEmitter &e)
	{
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
		setUniform("View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetView()));

		// draw all of the live particles
		for (int i = 0; i < e.firstInactiveParticle; ++i)
		{
			auto& curParticle = e.particles[i];
			setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(curParticle.transform));
			setUniform("Diffuse", nsfw::UNIFORM::TEX2, e.color, 0);

			glBindVertexArray(*e.mesh);
			glDrawElements(GL_TRIANGLES, *e.tris, GL_UNSIGNED_INT, 0);
		}
	}

	/*
	 * draw call for geometry
	*/
	void draw(Camera &c, const Geometry &g)	
	{
		setUniform("Projection",	nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
		setUniform("View",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetView()));
		setUniform("Model",			nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		setUniform("Diffuse",		nsfw::UNIFORM::TEX2,  g.diffuse,  0);

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, 0);
	}
};