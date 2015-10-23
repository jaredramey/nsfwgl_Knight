#include "ogl\gl_core_4_4.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "nsfw.h"


// Each Case should set up a uniform using the pass parameters
bool nsfw::RenderPass::setUniform(const char *name, nsfw::UNIFORM::TYPE type, const void *value, unsigned count, bool normalize) 
{
	//moved return to start of loop for now in order to allow program to run without error
	//while everything else gets fleshed out

	GLuint loc = glGetUniformLocation(*shader, name);
	GLuint val = *(const GLuint*)value;

	switch (type)
	{
	case nsfw::UNIFORM::FLO1: 
		//TODO_D("Setup float uniform!");
		
		glUniform1f(loc, *(const float*)value);
		return true;
		break;

	case nsfw::UNIFORM::FLO3: 
		//TODO_D("Setup vec3 uniform!");

		glUniform3fv(loc, 1, (const float*)value);
		return true;
		break;

	case nsfw::UNIFORM::FLO4: 
		//TODO_D("Setup vec4 uniform!");

		glUniform4fv(loc, 1, (const float*)value);
		return true;
		break;

	case nsfw::UNIFORM::MAT4: 
		//TODO_D("Setup mat4 uniform!");

		glUniformMatrix4fv(loc, count, GL_FALSE, (const float*)value );
		return true;
		break;

	case nsfw::UNIFORM::INT1: 
		//TODO_D("Setup integer uniform!");	 

		glUniform1i(loc, *(const GLuint*)value);
		return true;
		break;

	case nsfw::UNIFORM::TEX2: 
		//TODO_D("Setup texture2D uniform!"); 

		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, *(const GLuint*)value);
		glUniform1i(loc, count);
		return true;
		break;

	default:				  
		//TODO_D("INVALID Uniform type.");
		std::cout << "ERROR: INVALID UNIFORM TYPE" << std::endl;
		return false;
		break;
	}
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