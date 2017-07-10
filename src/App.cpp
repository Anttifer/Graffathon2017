#include "App.h"

#include "GLFunctions.h"
#include "GLUtils.h"
#include "Renderable.h"
#include "Updateable.h"
#include "Examples.h"
#include <cstdio>
#include <cstdint>

//--------------------

App::App(int /* argc */, char** /* argv */) :
	window_                (1440, 900, "impromptu"),
	time_                  ( (glfwSetTime(0), glfwGetTime()) ),
	clear_color_           (0.1, 0.1, 0.1),
	current_shader_        (nullptr),
	canvas_                (Mesh::canvas()),
	cube_                  (Mesh::cube()),

	wave_shader_           (GL::ShaderProgram(
		                       GL::ShaderObject::vertex_passthrough(),
		                       GL::ShaderObject::from_file(GL_FRAGMENT_SHADER, "shaders/wave_frag.glsl"))),
	wave_effect_           (&canvas_, &wave_shader_),

	rm_shader_             (create_raymarch_shader()),
	rm_effect_             (&canvas_, &rm_shader_),

	fcube_shader_          (GL::ShaderProgram::simple()),
	fcube_                 (&cube_, &fcube_shader_)
{
	// Disable vsync.
	glfwSwapInterval(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// This probably doesn't work, but worth asking anyway. :)
	glEnable(GL_LINE_SMOOTH);

	// renderables_.push_back(&rm_effect_);
	// renderables_.push_back(&wave_effect_);
	renderables_.push_back(&fcube_);
	updateables_.push_back(&fcube_);
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

		for (auto& updateable : updateables_)
			updateable->update(time_);

		render_scene(width, height);

		// Show the result on screen.
		glfwSwapBuffers(window_);

		// Poll events. Minimum FPS = 120.
		glfwWaitEventsTimeout(1 / 120.0f);
	}
}

void App::render_scene(int width, int height, GLuint framebuffer)
{
	GLint old_fbo; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(0, 0, width, height);

	for (auto& renderable : renderables_)
	{
		auto& shader = renderable->shader();
		if (current_shader_ != &shader)
		{
			glUseProgram(shader);
			current_shader_ = &shader;
		}

		auto proj = GLUtils::perspective(width, height);
		auto view = GLUtils::look_at({4.0f, 4.0f, 4.0f});

		RenderData data;
		data.width  = width;
		data.height = height;
		data.time   = time_;
		data.view   = &view;
		data.proj   = &proj;

		renderable->set_parameters(data);
		renderable->draw_arrays();
	}
}

GL::ShaderProgram App::create_raymarch_shader(void)
{
	auto vert       = GL::ShaderObject::vertex_passthrough();
	auto frag_scene = GL::ShaderObject::from_file(GL_FRAGMENT_SHADER, "shaders/rm_scene.glsl");
	auto frag_main  = GL::ShaderObject::from_file(GL_FRAGMENT_SHADER, "shaders/rm_frag.glsl");

	GL::ShaderProgram program;

	glAttachShader(program, vert);
	glAttachShader(program, frag_scene);
	glAttachShader(program, frag_main);

	if (!program.link())
	{
		std::cerr << "Shader program linking failed. Info log:" << std::endl << program.get_info_log();
		throw std::runtime_error("Shader program linking failed.");
	}

	return program;
}

//--------------------
