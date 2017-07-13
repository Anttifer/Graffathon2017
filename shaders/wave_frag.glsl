#version 430

layout(std140, binding = 0)
uniform BasicUniforms {
	mat4  uWorldToView;
	mat4  uProjection;
	ivec2 uScreenSize;
	float uTime;
};

in Data {
	vec3 vNormal;
	vec2 vTexCoord;
};

layout(location = 0) out vec4 fColor;

const float pi        = 3.14159265359;
const vec4  bgColor   = vec4(vec3(0.1), 1.0);
const vec4  waveColor = vec4(1.0, 0.6, 0.2, 1.0);
const vec4  axesColor = vec4(0.0, 1.0, 0.0, 1.0);

void main() {
	vec2 nPos  = (gl_FragCoord.xy - 0.5 * uScreenSize) / uScreenSize.y;
	ivec2 axes = uScreenSize / 2;

	float x = nPos.x;
	float y = nPos.y;

	float envelope  = sin(2 * pi * x - 2 * uTime);
	float carrier   = sin(20 * pi * x - 10 * uTime);
	float amplitude = 0.2;

	float dist = abs(y - amplitude*envelope*carrier);

	float alpha = 1.0 - smoothstep(0.01, 0.01 + fwidth(dist), dist);
	fColor = mix(bgColor, waveColor, alpha);

	if (any(equal(ivec2(gl_FragCoord.xy), axes)))
		fColor = axesColor;
}
