#ifndef APP_H
#define APP_H

#include "Window.h"
#include "ShaderCanvas.h"
#include "Mesh.h"
#include "GLObjects.h"
#include "Stuff.h"
#include "Examples.h"
#include <iostream>

class Renderable;
class Updateable;

//--------------------

class App
{
public:
	App (int argc, char* argv[]);
	App (const App&) = delete;

	App& operator= (const App&) = delete;

	void loop (void);

private:
	void render_scene (int width, int height, GLuint framebuffer = 0);

	GL::ShaderProgram create_raymarch_shader (void);

	MainWindow               window_;
	double                   time_;
	Eigen::Vector3f          clear_color_;
	GL::ShaderProgram*       current_shader_;
	std::vector<Renderable*> renderables_;
	std::vector<Updateable*> updateables_;

	ShaderCanvas             canvas_;
	Mesh                     cube_;

	GL::ShaderProgram  wave_shader_;
	CanvasEffect       wave_effect_;

	GL::ShaderProgram    rm_shader_;
	RaymarchEffect       rm_effect_;

	GL::ShaderProgram fcube_shader_;
	FancyCube         fcube_;
};
#endif // APP_H
