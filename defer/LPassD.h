#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> position, normal;
public:
	LPassD(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName), position("GPassPosition"), normal("GPassNormal") {}

	void prep() 
	{ 
		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc...");
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		glUseProgram(*shader);
	}

	void post()
	{ 
		//TODO_D("Unset any gl settings"); 
		glDisable(GL_BLEND);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void draw(const Camera &c, const LightD &l)
	{
		//BREADCRUMB
		/*do you use all of these in you lightpass shaders??*/
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getProjection()));
		setUniform("View",       nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.getView()));

		setUniform("CameraPos", nsfw::UNIFORM::FLO3, glm::value_ptr(c.transform[3]));

		setUniform("LightDirection", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.direction));
		setUniform("LightColor",     nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.color));

		setUniform("PositionMap", nsfw::UNIFORM::TYPE::TEX2, position, 0);

		/*verify the data you're sending here.*/
		setUniform("NormalMap", nsfw::UNIFORM::TYPE::TEX2, normal, 1);

		//setUniform("TexelScalar",    nsfw::UNIFORM::MAT4, glm::value_ptr(nsfw::Window::instance().getTexelAdjustmentMatrix()));

		/*
		* Uniforms to be set:
		* CameraPos      == DONE
		* LightDirection == DONE
		* LightColor	 == DONE
		* PositionMap    == DONE
		* NormalMap      == DONE
		*/

		unsigned quadVAOHandle  = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris    = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		//TODO_D("GL BindVAO/DrawElements with quad size and vao");

		glBindVertexArray(quadVAOHandle);
		//BREADCRUMB
		/*verify this call, not the same as in the tut. in framework all geometry made with ibo*/
		glDrawArrays(GL_TRIANGLES, 0, quadNumtris);
	}
};