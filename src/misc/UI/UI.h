#pragma once

struct GLFWwindow;

namespace UI
{
	bool initialize(GLFWwindow* pRawWindow);
	void finalize();

	void updateBegin();
	void updateEnd();
}
