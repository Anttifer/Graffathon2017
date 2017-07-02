#version 330

uniform float uTime;
uniform float uEpsilon = 0.001;

// Plane distance field.
float dPlane(in vec3 pos, in float height) {
        return pos.y - height;
}

// Sphere distance field.
float dSphere(in vec3 pos, in vec3 spherePos, in float radius) {
        return length(pos - spherePos) - radius;
}

// Box distance field.
float dBox(in vec3 pos, in vec3 boxPos, in vec3 boxCorner) {
        vec3 d = abs(pos - boxPos) - boxCorner;
        return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

// Smooth box distance field.
float dSmoothBox(in vec3 pos, in vec3 boxPos, in vec3 boxCorner, in float r) {
        vec3 d = abs(pos - boxPos) - boxCorner;
        return length(max(d, 0.0)) - r;
}

// Repeated position.
vec3 dRepeated(in vec3 pos, in vec3 lattice) {
        return mod(pos, lattice) - 0.5*lattice;
}

// Union of two distance fields.
float dUnion(in float d1, in float d2) {
        return min(d1, d2);
}

// Intersection of two distance fields.
float dIntersection(in float d1, in float d2) {
        return max(d1, d2);
}

// Difference of two distance fields.
float dDifference(in float d1, in float d2) {
        return max(d1, -d2);
}

vec3 posRepeated(vec3 pos, vec3 lattice) {
        vec3 d = pos + 0.5 * lattice;
        return mod(d, lattice) - 0.5 * lattice;
}

vec3 pRotatedX(vec3 pos, float angle) {
        float c = cos(angle);
        float s = sin(angle);

        mat3 rot = mat3(1.0, 0.0, 0.0,
                        0.0,   c,  -s,
                        0.0,   s,   c);

        return rot * pos;
}

vec3 pRotatedY(vec3 pos, float angle) {
        float c = cos(angle);
        float s = sin(angle);

        mat3 rot = mat3(  c, 0.0,   s,
                        0.0, 1.0, 0.0,
                         -s, 0.0,   c);

        return rot * pos;
}

vec3 pRotatedZ(vec3 pos, float angle) {
        float c = cos(angle);
        float s = sin(angle);

        mat3 rot = mat3(  c,  -s, 0.0,
                          s,   c, 0.0,
                        0.0, 0.0, 1.0);

        return rot * pos;
}

vec3 pTranslated(vec3 pos, vec3 trans) {
        return pos - trans;
}

// Combined distance field of the entire scene.
float dScene(in vec3 pos) {
        // vec3 boxPos = posRepeated(pos, vec3(6.0, 41, 6.0));
        vec3 boxPos = pos;

        vec3 boxCorner = vec3(1.0);
        vec3 absPos = abs(pos);
        float m = max(absPos.x, max(absPos.y, absPos.z));

        boxPos = pTranslated(boxPos, vec3(0.0, 0.0, 0.0));

        boxPos = pRotatedZ(boxPos, atan(1.0, sqrt(2.0)));
        boxPos = pRotatedX(boxPos, 3.141592654 / 4.0);

        //float box = dSmoothBox(boxPos, vec3(0), boxCorner, 0.1);
        float box = dBox(boxPos, vec3(0), boxCorner);
        float sphere1 = dSphere(boxPos, vec3(0.0), 1.3);
        float plane = dPlane(pos, -1 - uEpsilon);

        /*
        vec3 sphere5pos = vec3(0, 1, 0) + vec3(sin(2 * uTime), 0, cos(2 * uTime));
        vec3 sphere2pos = vec3(-1, 1, 0) + vec3(sin(3*uTime), cos(3*uTime), 0);
        vec3 sphere3pos = vec3(1, -1, 0) + vec3(cos(5*uTime), sin(5*uTime), 0);
        float sphere2 = dSphere(pos, sphere2pos, 0.3);
        float sphere3 = dSphere(pos, sphere3pos, 0.3);
        float sphere5 = dSphere(pos, sphere5pos, 0.5);

        float sphox = dDifference(box, sphere1);
        sphox = dDifference(sphox, sphere5);
        float spheres = dUnion(sphere2, sphere3);

        return dUnion(dUnion(sphox, spheres), plane);
        */

        return dUnion(plane, dDifference(box, sphere1));
}

// Normal vector for a point.
vec3 getNormal(in vec3 pos) {
        vec2 epsilon = vec2(uEpsilon, 0);
        return normalize(vec3(dScene(pos + epsilon.xyy) - dScene(pos - epsilon.xyy),
                              dScene(pos + epsilon.yxy) - dScene(pos - epsilon.yxy),
                              dScene(pos + epsilon.yyx) - dScene(pos - epsilon.yyx)));
}

//////////////////////////////////////////////

/*float dBox(vec3 pos, vec3 boxCorner) {
        vec3 d = abs(pos) - boxCorner;
        return min(0, max(d.x, max(dy, d.z))) + length(max(0, d));
}

float dSphere(vec3 pos, float radius) {
        return length(pos) - radius;
}

vec3 pRepeated(vec3 pos, vec3 lattice) {
        vec3 d = pos + 0.5 * lattice;
        return mod(d, lattice) - 0.5 * lattice;
}

vec3 pTranslated(vec3 pos, vec3 trans) {
        return pos - trans;
}

vec3 pRotatedX(vec3 pos, float angle) {
        float c = cos(angle);
        float s = sin(angle);

        mat3 rot = mat3(1.0, 0.0, 0.0,
                        0.0,   c,   s,
                        0.0,  -s,   c);

        return rot * pos;
}

vec3 pRotatedY(vec3 pos, float angle) {
        float c = cos(angle);
        float s = sin(angle);

        mat3 rot = mat3(  c, 0.0,  -s,
                        0.0, 1.0, 0.0,
                          s, 0.0,   c);

        return rot * pos;
}

vec3 pRotatedZ(vec3 pos, float angle) {
        float c = cos(angle);
        float s = sin(angle);

        mat3 rot = mat3(  c,   s, 0.0,
                         -s,   c, 0.0,
                        0.0, 0.0, 1.0);

        return rot * pos;
}

float dScene(vec3 pos) {

}*/
