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

#include "biquad.hpp"



static float s_param;
static dsp::BiQuad filter_l, filter_r;
static float resonance;
static float cutoff;
static float delay_feedback;


void MODFX_INIT(uint32_t platform, uint32_t api)
{
  delay_feedback = 0.0f;
  cutoff = 0.1f;
  resonance = 1.4041f;
  filter_l.flush();
  filter_l.mCoeffs.setSOLP(fx_tanpif(cutoff), resonance);
  filter_r.flush();
  filter_r.mCoeffs = filter_l.mCoeffs;
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
    *(my++) = fx_softclipf(0.1f, (*my++));
    *(my++) = fx_softclipf(0.1f, (*my++));
    *(sy++) = fx_softclipf(0.1f, (*sy++));
    *(sy++) = fx_softclipf(0.1f, (*sy++));
  }

}


void MODFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value);
  switch (index) {
  case k_user_modfx_param_time:
    break;
  case k_user_modfx_param_depth:
    s_param = valf;
    break;
  default:
    break;
  }
}

