#pragma once

#include <utils/Codes.h>

struct GLFWwindow;

struct InputHandle
{
	unsigned int currentFrame = 0;

	unsigned int frameCodes[KeyCode::KEY_LAST + ButtonCode::MOUSE_BUTTON_LAST + 2] = {};
	bool        activeCodes[KeyCode::KEY_LAST + ButtonCode::MOUSE_BUTTON_LAST + 2] = {};

	double mousePosX = 0;
	double mousePosY = 0;

	double mousePosDeltaX = 0;
	double mousePosDeltaY = 0;

	double scroll = 0;
};

extern InputHandle input;
extern InputHandle* pInput;

namespace Input
{
	bool initialize(GLFWwindow* pRawWindow);
	void update();

	bool pressed(int key);
	bool jpressed(int key);

	bool clicked(int button);
	bool jclicked(int button);
}