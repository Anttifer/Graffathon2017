#ifndef STUFF_H
#define STUFF_H

#include "GLObjects.h"
#include "Renderable.h"
#include "Updateable.h"
#include <Eigen/Geometry>

class ShaderCanvas;
class Mesh;

//--------------------

class CanvasEffect : public Renderable
{
public:
	CanvasEffect (ShaderCanvas*, GL::ShaderProgram*);

	void               set_parameters       (const RenderData&) override;
	void               draw_arrays          (void)              override;
	GL::ShaderProgram& shader               (void)              override { return *shader_; }

private:
	ShaderCanvas*      canvas_;
	GL::ShaderProgram* shader_;

	GLuint screen_size_uniform_;
	GLuint time_uniform_;
};

class RaymarchEffect : public Renderable
{
public:
	RaymarchEffect (ShaderCanvas*, GL::ShaderProgram*);

	void               set_parameters       (const RenderData&) override;
	void               draw_arrays          (void)              override;
	GL::ShaderProgram& shader               (void)              override { return *shader_; }

private:
	ShaderCanvas*      canvas_;
	GL::ShaderProgram* shader_;

	GLuint screen_size_uniform_;
	GLuint time_uniform_;

	GLuint eye_uniform_;
	GLuint forward_uniform_;
	GLuint right_uniform_;
	GLuint up_uniform_;
	GLuint far_uniform_;

	GLuint light_uniform_;
};

class FancyCube : public Renderable, public Updateable
{
public:
	FancyCube (Mesh*, GL::ShaderProgram*, GL::Texture* = nullptr);

	void               set_parameters       (const RenderData&) override;
	void               draw_arrays          (void)              override;
	GL::ShaderProgram& shader               (void)              override { return *shader_; }

	void update (double time) override;

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

private:
	Mesh*              cube_;
	GL::ShaderProgram* shader_;
	GL::Texture*       texture_;

	Eigen::Vector3f    position_;
	Eigen::Quaternionf attitude_;

	double last_second_;
	Eigen::Quaternionf source_;
	Eigen::Quaternionf target_;

	GLuint screen_size_uniform_;
	GLuint time_uniform_;
	GLuint model_uniform_;
	GLuint view_uniform_;
	GLuint projection_uniform_;
};

#endif // STUFF_H
