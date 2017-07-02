#version 330

uniform ivec2 uScreenSize;
uniform float uTime;

uniform vec3 uEye;
uniform vec3 uForward;
uniform vec3 uRight;
uniform vec3 uUp;

uniform float uFar;
uniform float uEpsilon = 0.001;

const int maxSteps = 512;

layout(location = 0) out vec4 cColor;


float dScene(vec3 pos);
vec3 getNormal(vec3 pos);

// Shaded colour of a point.
float smoothShadow(vec3 pos, vec3 norm) {
        vec3 shadowRayDir1 = normalize(vec3(0.5, 0.5, 0.5));
        vec3 shadowRayDir2 = normalize(vec3(0.5, 0.3, -0.3));

        float lightDist1 = 20.0;
        float lightDist2 = 20.0;
        float diffuseCoeff1 = clamp(dot(shadowRayDir1, norm), 0.0, 1.0);
        float diffuseCoeff2 = clamp(dot(shadowRayDir2, norm), 0.0, 1.0);

        float intensity1 = 0.7;
        float intensity2 = 0.3;
        float t = 0.0;
        float d = 1.0 / 0.0;
        vec3 shadowPos = pos;

        for (int i = 0; i < maxSteps; ++i) {
                if (d > uEpsilon && t < lightDist1) {
                        shadowPos = t * shadowRayDir1 + pos;
                        d = dScene(shadowPos);
                        t += d * (0.25 + abs(noise1(shadowPos)));

                        intensity1 = min(intensity1, 16.0 * d / t);
                }
                else
                        break;
        }

        if (d < uEpsilon)
                intensity1 = 0.0;

        t = 0.0;
        d = 1.0 / 0.0;
        for (int i = 0; i < maxSteps; ++i) {
                if (d > uEpsilon && t < lightDist2) {
                        shadowPos = t * shadowRayDir2 + pos;
                        d = dScene(shadowPos);
                        t += d * (0.25 + abs(noise1(shadowPos)));

                        intensity2 = min(intensity2, 8.0 * d / t);
                }
                else
                        break;
        }

        if (d < uEpsilon)
                intensity2 = 0.0;

        return diffuseCoeff1 * intensity1 + diffuseCoeff2 * intensity2;
}

void main() {
        vec2 nFragCoord = 2.0 * gl_FragCoord.xy / uScreenSize - vec2(1.0);
        vec3 rDir = nFragCoord.x * uRight + nFragCoord.y * uUp + uForward;
        rDir = normalize(rDir);

        float t = 0.0; float d = 1.0 / 0.0;
        vec3 pos = uEye;
        for (int i = 0; i < maxSteps; ++i) {
                if (d > uEpsilon && t < uFar) {
                        pos = t * rDir + uEye;
                        d = dScene(pos);
                        t += d * (0.4 + abs(noise1(pos)));
                }
                else
                        break;
        }

        vec3 materialColor;
        if (pos.y >= -1.0)
                materialColor = vec3(0.8, 0.8, 1.0);
        else
                materialColor = vec3(0.8, 0.8, 0.8);

        vec3 shadeNorm = getNormal(pos);
        vec3 shadePos = -uEpsilon * rDir + pos;

        float intensity = smoothShadow(shadePos, shadeNorm);

        vec3 color = 0.8 * intensity * materialColor;
        //color = mix(color, vec3(0.0), t / uFar);
        if (t > uFar / 2.0)
                color = mix(color, vec3(0.0), 2.0 * t / uFar - 1.0);
        cColor = vec4(color, 1.0);
}
