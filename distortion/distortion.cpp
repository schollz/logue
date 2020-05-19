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
    float t = t_param;
    float s = s_param;

    for (; my != my_e;) {
        float curmy = * my;
        float original = curmy;
        curmy = curmy * t; // gain
        // power gain
        // curmy = fasterpow2f(curmy,t);

        // // clip
        // if (curmy < -1 * s) {
        //     curmy = -0.5;
        // } else if (curmy > s) {
        //     curmy = 0.5;
        // } else {
        //     curmy = curmy * 1;
        // }

        // // clip boost
        // if (curmy < -1 * t) {
        //     curmy = -1;
        // } else if (curmy > t) {
        //     curmy = 1;
        // } else {
        //     curmy = curmy * 2;
        // }

        // wave fold
        if (curmy < -1 * t_param) {
            curmy = -1 * curmy - 1;
        } else if (curmy > t_param) {
            curmy = -1 * curmy + 1;
        } else {
            curmy = curmy * 1;
        }


        // normalize
        // if (curmy > 1) {
        //     curmy = 1;
        // } else if (curmy < -1) {
        //     curmy = -1;
        // }
        curmy = clipminmaxf(-1, curmy, 1);

        // apply wet-dry mix
        curmy = f32pair_linint(s_wet, original, curmy);

        *(my++) = curmy;
        *(my++) = curmy;
        *(sy++) = curmy;
        *(sy++) = curmy;
    }

}

void MODFX_PARAM(uint8_t index, int32_t value) {
    float valf = q31_to_f32(value);
    switch (index) {
    case k_user_modfx_param_time:
        t_param = valf * 10;
        break;
    case k_user_modfx_param_depth:
        s_param = valf;
        break;
    default:
        break;
    }
}
