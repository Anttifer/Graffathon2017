#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "GLObjects.h"
#include "RenderData.h"

//--------------------

class Renderable
{
public:
	virtual ~Renderable (void) = 0;

	virtual void set_parameters (const RenderData&) {}
	virtual void draw_arrays    (void)       = 0;

	virtual GL::ShaderProgram& shader (void) = 0;
};

inline Renderable::~Renderable(void) {}

#endif // RENDERABLE_H
