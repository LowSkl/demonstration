#include "Window.h"

#include <utils/Log.h>
#include <GLFW/glfw3.h>

bool window_initialized = false;

WindowHandle window;
WindowHandle* pWindow = &window;

void error_callback(int error_code, const char* description)
{
	LERROR("GLFW error {}: {}", error_code, description);
}

bool Window::initialize(WindowParams* params)
{
	if (window_initialized)
		return true;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		LCRITICAL("Can't init GLFW!");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	pWindow->pRawWindow = glfwCreateWindow(params->width, params->height, params->title, nullptr, nullptr);
	if (pWindow->pRawWindow == nullptr)
	{
		LCRITICAL("Can't init window!");
		return false;
	} glfwMakeContextCurrent(pWindow->pRawWindow);

	return window_initialized = true;
}

void Window::finalize()
{
	glfwDestroyWindow(pWindow->pRawWindow);
	glfwTerminate();

	window_initialized = false;
}

void Window::update()
{
	glfwSwapBuffers(pWindow->pRawWindow);
}

bool Window::is_opened()
{
	return !glfwWindowShouldClose(pWindow->pRawWindow);
}

bool Window::is_iconified()
{
	return glfwGetWindowAttrib(pWindow->pRawWindow, GLFW_ICONIFIED);
}

bool Window::is_focused()
{
	return glfwGetWindowAttrib(pWindow->pRawWindow, GLFW_FOCUSED);
}

bool Window::is_cursorLocked()
{
	return glfwGetInputMode(pWindow->pRawWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

void Window::set_cursor_pos(double x, double y)
{
	glfwSetCursorPos(pWindow->pRawWindow, x, y);
}

void Window::lock_cursor(bool flag)
{
	glfwSetInputMode(pWindow->pRawWindow, GLFW_CURSOR, flag ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Window::get_size(int* width, int* height)
{
	glfwGetWindowSize(pWindow->pRawWindow, width, height);
}