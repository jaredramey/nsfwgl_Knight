

#include <ogl\gl_core_4_4.h>
#include <glfw\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include "nsfw.h"

//this is for glfw debug 
void APIENTRY oglErrorDefaultCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	// if 'GL_DEBUG_OUTPUT_SYNCHRONOUS' is enabled, you can place a
	// breakpoint here and the callstack should reflect the problem location!

	std::cerr << message << std::endl;
}

void nsfw::Window::init(unsigned width, unsigned height)
{
	//TODO_D("Should create and set an active windowing context. ONLY GLFW! No GL!");

	//set width and height for later use
	this->width = width;
	this->height = height;

	//Initialize glfw
	glfwInit();

	//create title of the window
	const char* title = "nsfwgl Test";

	//tell glfw to use debug stuff
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	//create the window
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	//check to make sure window is created and functioning
	//if window isn't creating properly then let the console know
	if (window == nullptr)
		std::cout << "\n\n\n\n ERROR: WINDOW CREATION UNSUCCESSFUL \n\n\n\n" << std::endl;

	//set window to current context
	glfwMakeContextCurrent(window);

	//load functions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	//this is for glfw debug 
#ifdef _DEBUG
	if (glDebugMessageCallback)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(oglErrorDefaultCallback, nullptr);

		GLuint unusedIDs = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIDs, true);
	}
	else
	{
		std::cerr << "Failed to subscribe to glDebugMessageCallback." << std::endl;
	}
#endif
}

void nsfw::Window::step()
{
	//TODO_D("GLFW poll events and swap buffers is all that should really be here! No GL!");

	//swap buffers and poll events
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void nsfw::Window::term()
{
	//TODO();

	//Destroy window and terminate glfw
	glfwDestroyWindow(window);
	glfwTerminate();
}

float nsfw::Window::getTime() const
{
	//TODO();
	float cT = glfwGetTime();	//Current Time
	float nT = cT;				//New Time
	float dT = cT - nT;			//Delta Time

	return dT;	//Return Delta Time
}

bool nsfw::Window::getKey(unsigned k) const
{
	//TODO();

	if (glfwGetKey(window, k) != GLFW_PRESS)
		return false;

	else
		return true;
}

bool nsfw::Window::getShouldClose() const
{
	//TODO();

	return glfwWindowShouldClose(window);
}

unsigned nsfw::Window::getWidth() const
{
	//TODO();
	return width;
}

unsigned nsfw::Window::getHeight() const
{
	//TODO();
	return height;
}

glm::mat4 nsfw::Window::getTexelAdjustmentMatrix() const
{
	//TODO_D("Implemented, not tested.");

	glm::vec3 texelSize = 1.0f/glm::vec3(width,height,0);
	glm::vec3 halfTexel = texelSize * 0.5f; // bottom left

	// scale up to the appropriate size, then shift it to line up w/center of texels
	return glm::translate(halfTexel) * glm::scale(glm::vec3(1, 1, 1) - texelSize);	
}