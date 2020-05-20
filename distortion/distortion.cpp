/*
 * File: distortion.cpp
 *
 * Simple distortion
 *
 *
 *
 * 2018 (c) Korg
 * 2020 (c) Zack
 */

#include "usermodfx.h"


static float s_param, t_param;

void MODFX_INIT(uint32_t platform, uint32_t api) {

}

void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn, float *sub_yn,
                   uint32_t frames) {
    float *__restrict my = main_yn;
    const float *my_e = my + 2 * frames;
    float *__restrict sy = sub_yn;
    float tparam = t_param;
    float sparam = s_param;

    for (; my != my_e;) {
        float curmy = * my;
        float original = curmy;
        if (tparam < 1) {
            *(my++) = curmy;
            *(sy++) = curmy;
            continue;
        }
        curmy = curmy * tparam; // gain

        // // clip
        // if (curmy < -0.5f) {
        //     curmy = -0.5;
        // } else if (curmy > 0.5f) {
        //     curmy = 0.5;
        // } else {
        //     curmy = curmy * 1;
        // }

        // // clip boost
        // if (curmy < -0.5) {
        //     curmy = -1;
        // } else if (curmy > 0.5) {
        //     curmy = 1;
        // } else {
        //     curmy = curmy * 2;
        // }

        // wave fold
        if (curmy < -0.5f) {
            curmy = -1. * curmy - 1.f;
        } else if (curmy > 0.5f) {
            curmy = -1.f * curmy + 1.f;
        } else {
            curmy = curmy * 1.f;
        }


        // normalize
        curmy = clipminmaxf(-0.1f, curmy, 0.1f);


        curmy = linintf(sparam, original, curmy);

        // l-r will be applied each time
        *(my++) = curmy;
        *(sy++) = curmy;
    }

}

void MODFX_PARAM(uint8_t index, int32_t value) {
    float valf = q31_to_f32(value);
    switch (index) {
    case k_user_modfx_param_time:
        t_param = (valf) * 10.0f;
        break;
    case k_user_modfx_param_depth:
        s_param = valf;
        break;
    default:
        break;
    }
}
