#pragma once

struct GLFWwindow;

struct WindowHandle
{
	GLFWwindow* pRawWindow = nullptr;
};

extern WindowHandle window;
extern WindowHandle* pWindow;

struct WindowParams
{
	int width = 800;
	int height = 800;
	const char* title = "Demonstration";
};

namespace Window
{
	bool initialize(WindowParams* params);
	void finalize();

	void update();

	bool is_opened();
	bool is_iconified();
	bool is_focused();
	bool is_cursorLocked();

	void set_cursor_pos(double x, double y);
	void lock_cursor(bool flag);
	void get_size(int* width, int* height);
}
