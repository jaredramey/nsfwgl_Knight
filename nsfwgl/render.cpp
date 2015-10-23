#include "ogl\gl_core_4_4.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "nsfw.h"


// Each Case should set up a uniform using the pass parameters
bool nsfw::RenderPass::setUniform(const char *name, nsfw::UNIFORM::TYPE type, const void *value, unsigned count, bool normalize) 
{
	//moved return to start of loop for now in order to allow program to run without error
	//while everything else gets fleshed out

	GLint location = glGetUniformLocation(*shader, name);
	switch (type)
	{
	case nsfw::UNIFORM::FLO1:
		glUniform1f(location, *(GLfloat*)value);
		break;
	case nsfw::UNIFORM::FLO3:
		glUniform3fv(location, 1, (GLfloat*)value);
		break;
	case nsfw::UNIFORM::FLO4:
		glUniform4fv(location, 1, (GLfloat*)value);
		break;
	case nsfw::UNIFORM::MAT4:
		glUniformMatrix4fv(location, 1, false, (GLfloat*)value);
		break;
	case nsfw::UNIFORM::INT1:
		glUniform1i(location, *(GLint*)value);
		break;
	case nsfw::UNIFORM::TEX2:
		glUniform1i(location, count);
		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, *(GL_HANDLE*)value);
		break;
	default:
		std::cout << "Error setting uniform value, Bad Type.\n";
		return false;
	}

	return true;
}

// set GL state settings and globally accessible uniforms! Should be called before rendering occurs!
void nsfw::RenderPass::prep() 
{ 
	TODO_D("OVERRIDE: Setup the state of the card for this pass.");

	/*auto &s = Assets::instance();

	s.loadShader("testShader", "C:/Users/jared.ramey/Documents/GitHub/nsfwgl/Resources/Shaders/Comp_Vertex.txt", "C: / Users / jared.ramey / Documents / GitHub / nsfwgl / Resources / Shaders / Comp_Frag.txt");

	shader = "testShader";*/
}

// reset the GL state- undo what we did just in case.
void nsfw::RenderPass::post() 
{ 
	TODO_D("OVERRIDE: Clean up the state of the card."); 
}