#ifndef MESH4D_H
#define MESH4D_H

#include "GLObjects.h"
#include <Eigen/Geometry>
#include <vector>

//--------------------

class Mesh4D
{
public:
	Mesh4D (void) {}

	static Mesh4D pentachoron_wireframe (void);
	static Mesh4D octachoron_wireframe  (void);

public:
	template <typename EigenType>
	using aligned_vector = std::vector<EigenType, Eigen::aligned_allocator<EigenType>>;

	aligned_vector<Eigen::Vector4f> positions_;
};

#endif // MESH4D_H
