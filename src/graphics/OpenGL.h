#pragma once

struct GLFWwindow;
#include "buffers/VAO.h"

enum class Primitives : unsigned int
{
	POINTS         = 0x0000,
	LINES          = 0x0001,
	LINE_LOOP      = 0x0002,
	LINE_STRIP     = 0x0003,
	TRIANGLES      = 0x0004,
	TRIANGLE_STRIP = 0x0005,
	TRIANGLE_FAN   = 0x0006,
};

namespace OpenGL
{
	bool initialize(GLFWwindow* pRawWindow);

	void clear();
	void clear_color(float r, float g, float b, float a);

	void draw(const VAO& vao, Primitives primitive);
	double get_time();
}