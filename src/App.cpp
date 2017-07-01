#include "App.h"

#include "GLFunctions.h"
#include "GLUtils.h"
#include "Examples.h"
#include <cstdio>
#include <cstdint>

//--------------------

App::App(int /* argc */, char** /* argv */) :
	window_                (1440, 900, "impromptu"),
	time_                  ( (glfwSetTime(0), glfwGetTime()) ),

	clear_color_           (0.1, 0.1, 0.1)
{
	// Disable vsync.
	glfwSwapInterval(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// This probably doesn't work, but worth asking anyway. :)
	glEnable(GL_LINE_SMOOTH);
}

void App::loop(void)
{
	while (!glfwWindowShouldClose(window_))
	{
		time_ = glfwGetTime();

		int width, height;
		glfwGetFramebufferSize(window_, &width, &height);

		GL::clear_color(clear_color_);
		GL::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Examples::render_wave(width, height);

		// Show the result on screen.
		glfwSwapBuffers(window_);

		// Poll events. Minimum FPS = 100.
		glfwWaitEventsTimeout(1 / 100.0);
	}
}

//--------------------
