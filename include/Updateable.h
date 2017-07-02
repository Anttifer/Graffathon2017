#ifndef UPDATEABLE_H
#define UPDATEABLE_H

#include "GLObjects.h"
#include "RenderData.h"

//--------------------

class Updateable
{
public:
	virtual ~Updateable (void) {}

	virtual void update (double time) = 0;
};

#endif // UPDATEABLE_H
