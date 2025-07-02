#include "vec.h"


Vec3f VEC_GetMinus(const Vec3f vec1, const Vec3f vec2) {
    Vec3f vec;
    vec.x = vec1.x - vec2.x;
    vec.y = vec1.y - vec2.y;
    vec.z = vec1.z - vec2.z;
    return vec;
}
Vec3f VEC_GetCross(const Vec3f vec1, const Vec3f vec2) {
    Vec3f vec;
    vec.x = vec1.y * vec2.z - vec1.z * vec2.y;
    vec.y = vec1.z * vec2.x - vec1.x * vec2.z;
    vec.z = vec1.x * vec2.y - vec1.y * vec2.x;
    return vec;
}
Vec3f VEC_GetNormal(const Vec3f vecA, const Vec3f vecB, const Vec3f vecC) {
    const Vec3f vecAB = VEC_GetMinus(vecB, vecA);
    const Vec3f vecAC = VEC_GetMinus(vecC, vecA);
    const Vec3f vecNormal = VEC_GetCross(vecAB, vecAC);
    return vecNormal;
}