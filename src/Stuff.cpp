#include "Stuff.h"
#include "ShaderCanvas.h"
#include "Mesh.h"
#include "GLUtils.h"
#include <iostream>

CanvasEffect::CanvasEffect(ShaderCanvas* canvas, GL::ShaderProgram* shader) :
	canvas_ (canvas),
	shader_ (shader),

	screen_size_uniform_ (glGetUniformLocation(*shader_, "uScreenSize")),
	time_uniform_        (glGetUniformLocation(*shader_, "uTime"))
{}

void CanvasEffect::set_parameters(const RenderData& data)
{
	glUniform2i(screen_size_uniform_, data.width, data.height);
	glUniform1f(time_uniform_, data.time);
}

void CanvasEffect::draw_arrays(void)
{
	glBindVertexArray(canvas_->vao_);
	glDrawArrays(canvas_->primitive_type_, 0, canvas_->num_vertices_);
}

RaymarchEffect::RaymarchEffect(ShaderCanvas* canvas, GL::ShaderProgram* shader) :
	canvas_ (canvas),
	shader_ (shader),

	screen_size_uniform_ (glGetUniformLocation(*shader_, "uScreenSize")),
	time_uniform_        (glGetUniformLocation(*shader_, "uTime")),

	eye_uniform_         (glGetUniformLocation(*shader_, "uEye")),
	forward_uniform_     (glGetUniformLocation(*shader_, "uForward")),
	right_uniform_       (glGetUniformLocation(*shader_, "uRight")),
	up_uniform_          (glGetUniformLocation(*shader_, "uUp")),
	far_uniform_         (glGetUniformLocation(*shader_, "uFar")),

	light_uniform_       (glGetUniformLocation(*shader_, "uPointLight"))
{}

// Not updateable right now. We do everything here.
void RaymarchEffect::set_parameters(const RenderData& data)
{
	glUniform2i(screen_size_uniform_, data.width, data.height);
	glUniform1f(time_uniform_, data.time);

	// Camera.
	auto eye                = Eigen::Vector3f(3*std::sin(0.6*data.time), 2, 3*std::cos(0.6*data.time));
	auto target             = Eigen::Vector3f::Zero();
	auto up                 = Eigen::Vector3f(0, 1, 0);
	Eigen::Matrix3f FRU     = GLUtils::ray_look_at(eye, target, up, data.width, data.height);
	auto data_ptr           = FRU.data();

	auto light              = Eigen::Vector3f(4 * std::sin(PI / 4), 2.5, 4 * std::cos(PI / 4));

	glUniform3fv(eye_uniform_, 1, eye.data());
	glUniform3fv(forward_uniform_, 1, data_ptr);
	glUniform3fv(right_uniform_, 1, data_ptr + 3);
	glUniform3fv(up_uniform_, 1, data_ptr + 6);
	glUniform1f(far_uniform_, 50.0f);

	glUniform3fv(light_uniform_, 1, light.data());
}

void RaymarchEffect::draw_arrays(void)
{
	glBindVertexArray(canvas_->vao_);
	glDrawArrays(canvas_->primitive_type_, 0, canvas_->num_vertices_);
}

FancyCube::FancyCube(Mesh* cube, GL::ShaderProgram* shader, GL::Texture* texture) :
	cube_    (cube),
	shader_  (shader),
	texture_ (texture),

	position_ (0.0f, 0.0f, 0.0f),
	attitude_ (Eigen::Quaternionf::Identity()),

	last_second_ (-5.0),
	source_      (Eigen::Quaternionf::Identity()),
	target_      (Eigen::Quaternionf::Identity()),

	screen_size_uniform_ (glGetUniformLocation(*shader_, "uScreenSize")),
	time_uniform_        (glGetUniformLocation(*shader_, "uTime")),

	model_uniform_       (glGetUniformLocation(*shader_, "uModelToWorld")),
	view_uniform_        (glGetUniformLocation(*shader_, "uWorldToView")),
	projection_uniform_  (glGetUniformLocation(*shader_, "uProjection"))
{}

void FancyCube::set_parameters(const RenderData& data)
{
	glUniform2i(screen_size_uniform_, data.width, data.height);
	glUniform1f(time_uniform_, data.time);

	Eigen::Matrix4f model;
	model << attitude_.toRotationMatrix(), position_,
	         0, 0, 0,                      1;
	glUniformMatrix4fv(model_uniform_, 1, GL_FALSE, model.data());

	glUniformMatrix4fv(view_uniform_, 1, GL_FALSE, data.view->data());
	glUniformMatrix4fv(projection_uniform_, 1, GL_FALSE, data.proj->data());
}

void FancyCube::draw_arrays(void)
{
	glBindVertexArray(cube_->vao_);
	glDrawArrays(cube_->primitive_type_, 0, cube_->num_vertices_);
}

void FancyCube::update(double time)
{
	static auto slerp = [](double t, const Eigen::Quaternionf& a, const Eigen::Quaternionf& b)
	{
		auto dot = a.dot(b);

		dot = std::max(-1.0f, std::min(1.0f, dot));

		auto theta0 = std::acos(dot);
		auto theta = theta0 * t;

		// Eigen::Quaternionf c = b - a * Eigen::Quaternionf(dot, 0.0f, 0.0f, 0.0f);
		Eigen::Quaternionf c;
		c.coeffs() = b.coeffs() - (a * Eigen::Quaternionf(dot, 0.0f, 0.0f, 0.0f)).coeffs();
		c.normalize();

		auto cos_quat = Eigen::Quaternionf(std::cos(theta), 0.0f, 0.0f, 0.0f);
		auto sin_quat = Eigen::Quaternionf(std::sin(theta), 0.0f, 0.0f, 0.0f);

		// return a * cos_quat + c * sin_quat;
		Eigen::Quaternionf r = a * cos_quat;
		r.coeffs() += (c * sin_quat).coeffs();

		return r;
	};

	static auto ease = [](double t)
	{
		// double t2 = (t - 1.0) * (t - 1.0);
		// return 1.0 - t2 * t2 * std::cos(t * M_PI * 4.5);

		// return 1.0 - std::pow(2.0, -6.0f * t) * std::abs(std::cos(t * M_PI * 3.5));

		double t2;
		if (t < 0.5)
		{
			t2 = t * t;
			return 16.0 * t * t2 * t2;
		}
		else
		{
			--t;
			t2 = t * t;
			return 1.0 + 16.0 * t * t2 * t2;
		}
	};

	if (time - last_second_ > 1.0)
	{
		last_second_ = std::floor(time);
		source_ = target_;
		target_ = Eigen::Quaternionf::UnitRandom();
		if (source_.dot(target_) > 0.0f)
			target_ *= Eigen::Quaternionf{-1.0f,0.0f,0.0f,0.0f};
	}
	attitude_ = slerp(ease(time - last_second_), source_, target_);
}
