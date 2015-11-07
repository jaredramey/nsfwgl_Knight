#include "Keyboard.h"

typedef std::map<int, int>::iterator mapIt;

GLFWwindow* Keyboard::appWindow = nullptr;
std::map<int, int> Keyboard::keys;

void Keyboard::Init()
{
	if (nullptr == appWindow)
	{
		appWindow = glfwGetCurrentContext();
	}
	glfwSetKeyCallback(appWindow, Update);
}

bool Keyboard::IsKeyPressed(KEY keyToTest)
{
	return TestKeyState(keyToTest, GLFW_PRESS);
}

bool Keyboard::IsKeyReleased(KEY keyToTest)
{
	return TestKeyState(keyToTest, GLFW_RELEASE);
}

bool Keyboard::IsKeyRepeat(KEY keyToTest)
{
	return TestKeyState(keyToTest, GLFW_REPEAT);
}

void Keyboard::Update(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	keys[key] = action;
}

bool Keyboard::TestKeyState(const KEY key, const int state)
{
	mapIt it = keys.find(key);
	if (it == keys.end() || it->second != state)
	{
		return false;
	}
	return true;
}