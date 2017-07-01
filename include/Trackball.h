#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <Eigen/Geometry>
#include "CreateTrackingMember.h"

class Trackball
{
public:
	Trackball          (void)         : Trackball(1.0f, {0.0f, 0.0f, 0.0f})   {}
	explicit Trackball (float radius) : Trackball(radius, {0.0f, 0.0f, 0.0f}) {}
	Trackball          (float radius, const Eigen::Vector3f& position);

	void set_rotation_origin (const Eigen::Vector2f&);
	void rotate              (const Eigen::Vector2f&);

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
	Eigen::Vector3f ndc_to_relative_position (const Eigen::Vector2f&);

	CREATE_TRACKING_MEMBER(Eigen::Quaternionf, attitude)
	CREATE_TRACKING_MEMBER(Eigen::Vector3f, position)
	CREATE_TRACKING_MEMBER(float, radius)

	CREATE_TRACKING_MEMBER(Eigen::Matrix4f, view)
	CREATE_TRACKING_MEMBER(Eigen::Matrix4f, projection)

	Eigen::Quaternionf original_attitude_;
	Eigen::Vector3f    rotation_origin_;
};

#endif // TRACKBALL_H
