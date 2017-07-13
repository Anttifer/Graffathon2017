#version 430

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(std140, binding = 0)
uniform BasicUniforms {
	mat4  uWorldToView;
	mat4  uProjection;
	ivec2 uScreenSize;
	float uTime;
};

uniform mat3 uNormalToWorld = mat3(1.0);
uniform mat4 uModelToWorld = mat4(1.0);

out Data {
	vec3 vColor;
	vec3 vNormal;
	vec2 vTexCoord;
};

void main() {
	const vec3 distinctColors[6] = vec3[6](vec3(0, 0, 1), vec3(0, 1, 0), vec3(0, 1, 1),
	                                       vec3(1, 0, 0), vec3(1, 0, 1), vec3(1, 1, 0));

	vColor = distinctColors[gl_VertexID % 6];
	vNormal = normalize(uNormalToWorld * aNormal);
	vTexCoord = aTexCoord;

	gl_Position = uProjection * uWorldToView * uModelToWorld * vec4(aPosition, 1.0);
}
