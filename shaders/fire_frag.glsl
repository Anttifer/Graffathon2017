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
const vec4  fireColor = vec4(0.8, 0.6, 0.3, 1.0);

float rand(vec2 co) {
	  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
} // (sic)

vec2 gradient(ivec2 point) {
	float angle = 2 * pi * rand(point);
	return vec2(cos(angle), sin(angle));
}

float perlin2(vec2 v, float cellWidth) {
	v /= cellWidth;

	int x0 = int(floor(v.x));
	int y0 = int(floor(v.y));
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float d00 = dot(gradient(ivec2(x0, y0)), v - vec2(x0, y0));
	float d10 = dot(gradient(ivec2(x1, y0)), v - vec2(x1, y0));
	float d01 = dot(gradient(ivec2(x0, y1)), v - vec2(x0, y1));
	float d11 = dot(gradient(ivec2(x1, y1)), v - vec2(x1, y1));

	float sx = smoothstep(0, 1, v.x - x0);
	float sy = smoothstep(0, 1, v.y - y0);

	float ix0 = mix(d00, d10, sx);
	float ix1 = mix(d01, d11, sx);

	return sqrt(2.0) * mix(ix0, ix1, sy);
}

float fbm(vec2 v, float cellWidth, int iterations) {
	float result = 0.0;
	float amplitude = 1.0;

	float cellWidthFalloff = 0.5;
	float amplitudeFalloff = 0.5;

	for (int i = 0; i < iterations; ++i) {
		result += amplitude * perlin2(v + i, cellWidth);
		cellWidth *= cellWidthFalloff;
		amplitude *= amplitudeFalloff;
	}

	return result;
}

void main() {
	vec2 nPos = (gl_FragCoord.xy - 0.5 * uScreenSize) / uScreenSize.y;

	vec2  timeVec     = 0.7 * vec2(0.0, -uTime);
	float xDistortion = 0.1 * fbm(nPos - 7 + timeVec, 0.2, 4);
	float yDistortion = 0.1 * fbm(nPos + 7 + timeVec, 0.2, 4);
	vec2  pos         = nPos + vec2(xDistortion, yDistortion);

	fColor = mix(bgColor, fireColor, 0.5 - 5*nPos.y + 2*fbm(pos + 0.5 * timeVec, 0.4, 4));
}
