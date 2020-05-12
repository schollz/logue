/*
 * File: tremoxd.cpp
 *
 * Tremolo effect for Minilogue XD mod fx
 *
 * Param time: Tremolo effect LFO rate
 * Param depth: Tremolo effect depth
 * 
 */

#include "usermodfx.h"
#include "osc_api.h"
#include "simplelfo.hpp"

static dsp::SimpleLFO lfo;

float tremolo_depth;

/* Implementation of the initialization callback */
void MODFX_INIT(uint32_t platform, uint32_t api)
{
  tremolo_depth = 0.0;
  lfo.reset();
  lfo.setF0(0.0, k_samplerate_recipf);
}


/**
 * Implementation of the process callback
 * Ref: https://korginc.github.io/logue-sdk/ref/minilogue-xd/v1.1-0/html/group__modfx__inst.html#gaea822f104394d2f1d9139568d18d7994
 *
 * @param main_xn Input sample buffer for main timbre
 * @param main_yn Output sample buffer for main timbre
 * @param sub_xn  Input sample buffer for sub timbre
 * @param sub_yn  Output sample buffer for sub timbre
 * @param frames  Size of buffers. (2 samples per frames)
 *
 * Note: sub_xn and sub_yn are not used here, as the Minilogue XD has no sub timbre
 */
void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn,  float *sub_yn,
                   uint32_t frames)
{

  // main input
  const float * mx = main_xn;

  // main output
  float * __restrict my = main_yn;

  // one sample per channel (L/R)
  const float * my_e = my + 2*frames;

  float processed_mx;

  for (; my != my_e; ) {

    float tremolo_factor = 1.0 - tremolo_depth * lfo.sine_bi();

    processed_mx = *(mx++) * tremolo_factor; // use L channel sample as input (mono)
    mx++; //ignore R channel sample

    // write to both channels (L/R)
    *(my++) = processed_mx;
    *(my++) = processed_mx;

    lfo.cycle();
  }

}

/**
 * Implementation of the param callback that gets called when the user changes the time or depth knob
 */
void MODFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value); // valf is in range (0.0 - 1.0)
  switch (index) {
  case k_user_modfx_param_time:
      lfo.setF0(valf * 10.0, k_samplerate_recipf);
    break;
  case k_user_modfx_param_depth:
    tremolo_depth = valf;
    break;
  default:
    break;
  }
}
