#ifndef UPDATEABLE_H
#define UPDATEABLE_H

#include "GLObjects.h"
#include "RenderData.h"

//--------------------

class Updateable
{
public:
	virtual ~Updateable (void) = 0;

	virtual void update (double time) = 0;
};

inline Updateable::~Updateable(void) {}

#endif // UPDATEABLE_H
