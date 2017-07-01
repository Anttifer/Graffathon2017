#include "Trackball.h"

Trackball::Trackball(float radius, const Eigen::Vector3f& position) :
	attitude_default_   (Eigen::Quaternionf::Identity()),
	position_default_   (position),
	radius_default_     (radius),

	view_default_       (Eigen::Matrix4f::Identity()),
	projection_default_ (Eigen::Matrix4f::Identity())
{}

static float spherical_height(float radius, const Eigen::Vector2f& p)
{
	const auto r2 = radius * radius;
	return std::sqrt(r2 - p.squaredNorm());
}

static float hyperbolic_height(float radius, const Eigen::Vector2f& p)
{
	const auto r2 = radius * radius;
	return r2 / (2.0f * p.norm());
}

static float trackball_height(float radius, const Eigen::Vector2f& p)
{
	const auto r2 = radius * radius;
	if (2.0f * p.squaredNorm() > r2)
		return hyperbolic_height(radius, p);
	else
		return spherical_height(radius, p);
}

Eigen::Vector3f Trackball::ndc_to_relative_position(const Eigen::Vector2f& p)
{
	Eigen::Vector4f ball_view_pos = (*view_) * position_->homogeneous();

	Eigen::Vector4f ball_clip_pos = (*projection_) * ball_view_pos;
	const float z = ball_clip_pos.z();
	const float w = ball_clip_pos.w();

	Eigen::Vector4f p_clip_pos = { w * p.x(), w * p.y(), z, w };
	Eigen::Vector4f p_view_pos = projection_->inverse() * p_clip_pos;
	// No need to use .hnormalized here due to how we constructed p_clip_pos. :)

	p_view_pos.z() += trackball_height(*radius_, (p_view_pos - ball_view_pos).head<2>());

	return (view_->inverse() * p_view_pos).head<3>() - (*position_);
}

void Trackball::set_rotation_origin(const Eigen::Vector2f& p)
{
	original_attitude_ = *attitude_;
	rotation_origin_   = ndc_to_relative_position(p);
}

void Trackball::rotate(const Eigen::Vector2f& p)
{
	const auto target   = ndc_to_relative_position(p);
	const auto rotation = Eigen::Quaternionf::FromTwoVectors(rotation_origin_, target);

	*attitude_ = rotation * original_attitude_;
}
