#include "GLUtils.h"

#include <iostream>

using namespace Eigen;

//--------------------

namespace GLUtils
{
Matrix4f perspective(int width, int height, float fov, float near, float far)
{
	float n = near;
	float f = far;
	float AR = width / (float)height;
	float r = std::tan(fov / 2.0f) * n;
	float l = -r;
	float t = r / AR;
	float b = -t;

	return (Matrix4f() << 2.0f*n/(r-l) , 0.0f         ,  (r+l)/(r-l) ,  0.0f           ,
	                      0.0f         , 2.0f*n/(t-b) ,  (t+b)/(t-b) ,  0.0f           ,
	                      0.0f         , 0.0f         , -(f+n)/(f-n) , -2.0f*f*n/(f-n) ,
	                      0.0f         , 0.0f         , -1.0f        ,  0.0f           ).finished();

}

Matrix<float, 5, 5> perspective_4d(float fov)
{
	float adj = 1.0f / std::tan(fov / 2.0f);

	// TODO: 4-depth.
	return (Matrix<float, 5, 5>() << adj,   0,   0,  0, 0,
	                                   0, adj,   0,  0, 0,
	                                   0,   0, adj,  0, 0,
	                                   0,   0,   0,  1, 0,
	                                   0,   0,   0, -1, 0).finished();
}

//--------------------

Matrix4f look_at(const Vector3f& eye, const Vector3f& target, const Vector3f& up_hint)
{
	Vector3f z = (eye - target).normalized();
	Vector3f y = (up_hint - up_hint.dot(z) * z).normalized();
	Vector3f x = y.cross(z).normalized();

	Matrix4f rot;
	rot << x , y , z , Vector3f::Zero() ,
	       0 , 0 , 0 , 1                ;
	rot.transposeInPlace();

	Matrix4f trans = Matrix4f::Identity();
	trans.col(3) << -eye ,
	                 1   ;

	return rot * trans;
}

// TODO: Generalize this.
static Vector4f cross4d(const Vector4f& x, const Vector4f& y, const Vector4f& z)
{
	Matrix<float, 4, 3> input;
	input << x, y, z;

	float x_det = input.bottomRows<3>().determinant();
	float y_det = (Matrix3f() << input.topRows<1>(), input.bottomRows<2>()).finished().determinant();
	float z_det = (Matrix3f() << input.topRows<2>(), input.bottomRows<1>()).finished().determinant();
	float w_det = input.topRows<3>().determinant();

	return - x_det * Vector4f::UnitX() + y_det * Vector4f::UnitY()
	       - z_det * Vector4f::UnitZ() + w_det * Vector4f::UnitW();
}

Matrix<float, 5, 5> look_at_4d(const Vector4f& eye, const Vector4f& target,
                    const Vector4f& up_hint, const Vector4f& back_hint)
{
	Vector4f w = (eye - target).normalized();

	// Modified Gram-Schmidt to obtain an orthonormal basis.
	Vector4f y = (up_hint - up_hint.dot(w) * w).normalized();
	Vector4f z = (back_hint - back_hint.dot(w) * w);
	z          = z - z.dot(y) * y;
	z.normalize();

	// Negative for right-handedness (the permutation yzwx->xyzw is odd).
	Vector4f x = -cross4d(y, z, w);

	Matrix<float, 5, 5> rot;
	rot << x, y, z, w, Vector4f::Zero(),
	       0, 0, 0, 0, 1;
	rot.transposeInPlace();

	Matrix<float, 5, 5> trans = Matrix<float, 5, 5>::Identity();
	trans.col(4) << -eye,
	                 1;

	return rot * trans;
}

//--------------------

Matrix3f ray_look_at(const Vector3f& eye, const Vector3f& target, const Vector3f& up_hint,
                     int width, int height, float fov)
{
	Vector3f forward = (target - eye).normalized();
	Vector3f up = (up_hint - up_hint.dot(forward) * forward).normalized();
	Vector3f right = forward.cross(up);

	float AR = width / (float)height;
	float r = std::tan(fov / 2);
	float t = r / AR;

	right *= r;
	up *= t;

	return (Matrix3f() << forward, right, up).finished();
}

//--------------------

GL::ShaderProgram supercool_shader(void)
{
	return GL::ShaderProgram(
		"#version 330\n"
		GL_SHADER_SOURCE(
			layout(location = 0) in vec3 aPosition;

			void main() {
				gl_Position = vec4(aPosition, 1);
			}
		),
		"#version 330\n"
		GL_SHADER_SOURCE(
			uniform ivec2 uScreenSize;
			uniform float uTime;

			out vec4 cColor;

			void main() {
				float pi = 3.14159265359;
				float AR = uScreenSize.x / float(uScreenSize.y);
				vec2 axes = uScreenSize / 2;

				vec2 nPos = gl_FragCoord.xy / uScreenSize - 0.5;
				nPos.x *= AR;

				float envelope = sin(2 * pi * nPos.x - 2 * uTime);
				float carrier  = sin(20 * pi * nPos.x - 10 * uTime);
				float amplitude = 0.4;

				int wave = int(axes.y * (amplitude * envelope * carrier + 1.0));

				if (any(equal(ivec2(gl_FragCoord.xy), ivec2(axes))))
					cColor = vec4(0, 1, 0, 1);
				else if (int(gl_FragCoord.y) == wave)
					cColor = vec4(1, 0.6, 0.2, 1);
				else
					cColor = vec4(0.1, 0.1, 0.1, 1);
			}
		)
	);
}
} // namespace GLUtils
