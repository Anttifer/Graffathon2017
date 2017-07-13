#version 430

uniform bool uTextureFlag = false;
uniform sampler2D uTextureSampler;

in Data {
	vec3 vColor;
	vec3 vNormal;
	vec2 vTexCoord;
};

layout(location = 0) out vec4 fColor;

void main() {
	fColor = vec4(vColor, 1.0);

	if (uTextureFlag)
		fColor = texture(uTextureSampler, vTexCoord);
}
