/*
 * File: lfo.cpp
 *
 * Simple runtime test using LFO class as audio rate oscillator
 *
 * 
 * 
 * 2018 (c) Korg
 *
 */

#include "usermodfx.h"


static float s_param,t_param;

void MODFX_INIT(uint32_t platform, uint32_t api)
{

}

void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames)
{
  float * __restrict my = main_yn;
  const float * my_e = my + 2*frames;
  float * __restrict sy = sub_yn;
float t = t_param;
float s = s_param;

  for (; my != my_e; ) {
    float curmy = *my;
    // curmy = curmy*t_param;
    if (t > 0) {

if (s < 0.333) {
    // clip 
    if (curmy < -1*s) {
      curmy = -0.5;
    } else if (curmy > s) {
      curmy = 0.5;
    } else {
      curmy = curmy*1;
    }  
} else if (s < 0.666) {
    // clip boost 
    if (curmy < -1*t) {
      curmy = -1;
    } else if (curmy > t) {
      curmy = 1;
    } else {
      curmy = curmy*2;
    }  
} else  {
    // wave fold 
    if (curmy < -1*t_param) {
      curmy = -1*curmy-1;
    } else if (curmy > t_param) {
      curmy = -1*curmy+1;
    } else {
      curmy = curmy*1;
    }  
}

    // normalize
    if (curmy > 1) {
      curmy = 1;
    } else if (curmy < -1) {
      curmy = -1;
    }


}


    *(my++) = curmy;
    *(my++) = curmy;
    *(sy++) = curmy;
    *(sy++) = curmy;
  }

}


void MODFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value);
  switch (index) {
  case k_user_modfx_param_time:
    t_param = valf/4;
    break;
  case k_user_modfx_param_depth:
    s_param = valf;
    break;
  default:
    break;
  }
}

