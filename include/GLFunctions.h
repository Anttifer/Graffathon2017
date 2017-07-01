#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H

#include <GL/glew.h>
#include "GLObjects.h"
#include <Eigen/Geometry>

//--------------------

namespace GL
{
inline void clear(GLbitfield mask, GLuint framebuffer = 0)
{
	GLint old_fbo; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(mask);
	glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

inline void clear_color(const Eigen::Vector3f& c)
{
	glClearColor(c.x(), c.y(), c.z(), 0);
}

inline void clear_color(const Eigen::Vector4f& c)
{
	glClearColor(c.x(), c.y(), c.z(), c.w());
}

inline void clear_color(float x, float y, float z)
{
	glClearColor(x, y, z, 0);
}

inline void clear_color(float x, float y, float z, float w)
{
	glClearColor(x, y, z, w);
}

size_t internal_format_size(GLenum format);

void tex_to_png(const Texture& texture, const char* filename);
} // namespace GL

#endif // GLFUNCTIONS_H
