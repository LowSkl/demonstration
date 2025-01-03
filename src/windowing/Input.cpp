#include "Input.h"

#include <GLFW/glfw3.h>

bool input_initialized = false;

InputHandle input;
InputHandle* pInput = &input;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	pInput->activeCodes[key] = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
	pInput->frameCodes[key] = pInput->currentFrame;
}

void button_callback(GLFWwindow* window, int button, int action, int mods)
{
	pInput->activeCodes[KeyCode::KEY_LAST + button] = action == GLFW_PRESS;
	pInput->frameCodes[KeyCode::KEY_LAST + button] = pInput->currentFrame;
}

void pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	pInput->mousePosDeltaX = xpos - pInput->mousePosX;
	pInput->mousePosDeltaY = ypos - pInput->mousePosY;

	pInput->mousePosX = xpos;
	pInput->mousePosY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	pInput->scroll = yoffset;
}

bool Input::initialize(GLFWwindow* pRawWindow)
{
	if (input_initialized) return true;

	glfwSetKeyCallback(pRawWindow, key_callback);
	glfwSetMouseButtonCallback(pRawWindow, button_callback);
	glfwSetCursorPosCallback(pRawWindow, pos_callback);
	glfwSetScrollCallback(pRawWindow, scroll_callback);

	return input_initialized = true;
}

void Input::update()
{
	++pInput->currentFrame;
	glfwPollEvents();
}

bool Input::pressed(int key) { return pInput->activeCodes[key]; }
bool Input::jpressed(int key) { return pInput->activeCodes[key] && pInput->frameCodes[key] == pInput->currentFrame; }

bool Input::clicked(int button) { return pInput->activeCodes[KeyCode::KEY_LAST + button]; }
bool Input::jclicked(int button) { return pInput->activeCodes[KeyCode::KEY_LAST + button] && pInput->frameCodes[KeyCode::KEY_LAST + button] == pInput->currentFrame; }