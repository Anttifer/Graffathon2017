#ifndef GLUTILS_H
#define GLUTILS_H

#include <Eigen/Geometry>
#include "GLObjects.h"
#define GL_SHADER_SOURCE(CODE) #CODE
#define PI 3.1415926536f

//--------------------

namespace GLUtils
{
Eigen::Matrix4f perspective(int width, int height,
                            float fov = PI / 2.0f, float near = 0.1f, float far = 15.0f);

Eigen::Matrix<float, 5, 5> perspective_4d(float fov);

Eigen::Matrix4f look_at(const Eigen::Vector3f& eye,
                        const Eigen::Vector3f& target  = {0, 0, 0},
                        const Eigen::Vector3f& up_hint = {0, 1, 0});

Eigen::Matrix<float, 5, 5> look_at_4d(const Eigen::Vector4f& eye,
                                      const Eigen::Vector4f& target    = {0, 0, 0, 0},
                                      const Eigen::Vector4f& up_hint   = {0, 1, 0, 0},
                                      const Eigen::Vector4f& back_hint = {0, 0, 1, 0});


Eigen::Matrix3f ray_look_at(const Eigen::Vector3f& eye,
                            const Eigen::Vector3f& target,
                            const Eigen::Vector3f& up_hint,
                            int width, int height,
                            float fov = PI / 2.0f);

GL::ShaderProgram supercool_shader(void);
} // namespace GLUtils

#endif // GLUTILS_H
