#ifndef APP_H
#define APP_H

#include "Window.h"
#include "ShaderCanvas.h"
#include "GLObjects.h"
#include "Examples.h"

//--------------------

class App
{
public:
	App (int argc, char* argv[]);
	App (const App&) = delete;

	App& operator= (const App&) = delete;

	void loop (void);

private:
	// Framework objects.
	MainWindow    window_;
	double        time_;
	ShaderCanvas  canvas_;

	// Parameters and options.
	Eigen::Vector3f clear_color_;
};
#endif // APP_H
