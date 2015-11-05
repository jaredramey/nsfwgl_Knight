#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> position, normal, shadow;
public:
	LPassD(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName), position("GPassPosition"),
			normal("GPassNormal"), shadow("ShadowMap") {}

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
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
		setUniform("View",       nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetView()));
		
		setUniform("LightProjection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(l.getProjection()));
		setUniform("LightView", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(l.getView()));

		
		//unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		//unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glm::mat4 ClipToUV(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f);

		setUniform("ClipToUV", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(ClipToUV));

		setUniform("CameraPos", nsfw::UNIFORM::FLO3, glm::value_ptr(c.GetWorldTransform()[3]));

		setUniform("LightDirection", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.direction));
		setUniform("LightColor",     nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.color));

		setUniform("PositionMap", nsfw::UNIFORM::TYPE::TEX2, position, 0);
		setUniform("NormalMap", nsfw::UNIFORM::TYPE::TEX2, normal, 1);
		setUniform("ShadowMap", nsfw::UNIFORM::TYPE::TEX2, shadow, 2);

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
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
	}
};