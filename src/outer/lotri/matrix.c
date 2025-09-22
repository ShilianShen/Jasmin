#include "matrix.h"


// VEC =================================================================================================================
Vec3f LOTRI_AtvVec(const Vec3f a, const Vec3f b, const float t, const AtvFunc atv) {
    Vec3f result;
    for (int i = 0; i < 3; i++) {
        result.arr[i] = a.arr[i] + atv(t) * (b.arr[i] - a.arr[i]);
    }
    return result;
}


Vec3f LOTRI_GetDelayVecVec(const DelayVec3f delay) {
    if (delay.t1 == delay.t2) return delay.v2;

    const float t = (float)SDL_GetTicks() / 1000;
    const float rate = (t - delay.t1) / (delay.t2 - delay.t1);
    // DEBUG_SendMessageR("AA%.2f, %.2f, %.2f\n", t, rate, 0);
    return LOTRI_AtvVec(delay.v1, delay.v2, rate, BASIC_AtvRank1);
}
bool LOTRI_SetDelayVec(DelayVec3f* delay, const Vec3f v2, const float time) {
    if (delay == NULL || BASIC_GetVecEqual(delay->v2, v2) == true) return false;

    const float t = (float)SDL_GetTicks() / 1000;
    if (delay->block == true && t < delay->t2) return false;

    delay->v1 = LOTRI_GetDelayVecVec(*delay);
    delay->t1 = t;
    delay->t2 = t + time;
    delay->v2 = v2;
    return true;
}





// MAT =================================================================================================================



// VERTEX & FACE =======================================================================================================
bool LOTRI_LoadV3M4(const int N, Vec3f vec_in[N], const Mat4f mat, Vec4f vec_out[N], const bool w) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 4; j++) {
            vec_out[i].arr[j] = 0;
            for (int k = 0; k < 3; k++) {
                vec_out[i].arr[j] += vec_in[i].arr[k] * mat.m[k][j];
            }
            if (w) {
                vec_out[i].arr[j] += mat.m[3][j];
            }
        }
    }
    return true;
}


