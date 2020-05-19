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

#include "simplelfo.hpp"

static dsp::SimpleLFO s_lfo;


static uint8_t s_lfo_wave;
static float s_param;
static const float s_fs_recip = 1.f / 48000.f;

void MODFX_INIT(uint32_t platform, uint32_t api)
{
  s_lfo.reset();
  s_lfo.setF0(0.1f,s_fs_recip);
}

void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames)
{
  float * __restrict my = main_yn;
  const float * my_e = my + 2*frames;
  float * __restrict sy = sub_yn;

  const float p = s_param;
  
  for (; my != my_e; ) {
    s_lfo.cycle();

    float wave;
    wave = s_lfo.sine_uni();
    float curmy = *my;
    *(my++) = curmy*(1-s_param) + curmy*(1.0f-wave)*(s_param);
    *(my++) = curmy*(1-s_param) + curmy*(wave)*(s_param);
    *(sy++) = curmy*(1-s_param) + curmy*(1.0f-wave)*(s_param);
    *(sy++) = curmy*(1-s_param) + curmy*(wave)*(s_param);
  }

}


void MODFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value);
  switch (index) {
  case k_user_modfx_param_time:
    s_lfo.setF0(valf,s_fs_recip);
    break;
  case k_user_modfx_param_depth:
    s_param = valf;
    break;
  default:
    break;
  }
}

