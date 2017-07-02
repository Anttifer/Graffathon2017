#ifndef RENDERDATA_H
#define RENDERDATA_H

#include <Eigen/Geometry>

//--------------------

struct RenderData
{
	int    width  = 0;
	int    height = 0;
	double time   = 0.0;

	Eigen::Matrix4f* view = nullptr;
	Eigen::Matrix4f* proj = nullptr;
};

#endif // RENDERDATA_H
