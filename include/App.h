#ifndef APP_H
#define APP_H

#include "Window.h"
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

	MainWindow               window_;
	double                   time_;
	Eigen::Vector3f          clear_color_;
	GL::ShaderProgram*       current_shader_;
	std::vector<Renderable*> renderables_;
	std::vector<Updateable*> updateables_;

	Mesh                     canvas_;
	Mesh                     cube_;

	struct BasicUniforms
	{
		Eigen::Matrix<float,4,4,Eigen::DontAlign> view;
		Eigen::Matrix<float,4,4,Eigen::DontAlign> proj;
		Eigen::Matrix<int,2,1,Eigen::DontAlign>   screen_size;
		float                                     time;
	};
	GL::UBO<BasicUniforms> basic_uniforms_;

	GL::ShaderProgram  wave_shader_;
	CanvasEffect       wave_effect_;

	GL::ShaderProgram  fire_shader_;
	CanvasEffect       fire_effect_;

	GL::ShaderProgram    rm_shader_;
	RaymarchEffect       rm_effect_;

	GL::ShaderProgram fcube_shader_;
	FancyCube         fcube_;
};
#endif // APP_H
