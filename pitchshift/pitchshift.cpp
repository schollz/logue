#include "common.h"
#include "delayline.hpp"
#include "biquad.hpp"

#define BUFFER_SIZE 48000

static dsp::DualDelayLine buffer;
static float delay_time;
static float delay_feedback;
static float pitch;
static float buffer_index;
static uint8_t is_pitch_shifter;

static dsp::BiQuad filter_l, filter_r;
static float resonance;
static float cutoff;
static const float fade_threshold_time = BUFFER_SIZE / 400;

static __sdram f32pair_t buffer_ram[BUFFER_SIZE];

void DELFX_INIT(uint32_t platform, uint32_t api) {
  buffer.setMemory(buffer_ram, BUFFER_SIZE);
  delay_time = 0.0f;
  delay_feedback = 0.0f;
  pitch = 1.4983070768766815f;
  buffer_index = 0.0f;
  is_pitch_shifter = 0;
  cutoff = 0.1f;
  resonance = 1.4041f;

  filter_l.flush();
  filter_l.mCoeffs.setSOLP(fx_tanpif(cutoff), resonance);
  filter_r.flush();
  filter_r.mCoeffs = filter_l.mCoeffs;
}

void DELFX_PROCESS(float *xn, uint32_t frames) {
  float * __restrict x = xn;
  const float * x_e = x + 2*frames;

  for (; x != x_e ; x+=2) {
    f32pair_t lr = buffer.readFrac(buffer_index);
    *x = fx_softclipf(0.05f, (*x + filter_l.process_so(lr.a * delay_feedback)));
    *(x+1) = fx_softclipf(0.05f, *(x+1) + filter_r.process_so(lr.b * delay_feedback));
    // remove this
    // if (buffer_index < fade_threshold_time) {
    //   float rate = (fade_threshold_time - buffer_index) / fade_threshold_time;
    //   *x = (*x) * (1.0f - rate);
    //   // *(x+1) = (*(x+1)) * (1.0f - rate);
    // } else if (buffer_index >= (delay_time - fade_threshold_time)) {
    //   float rate = (delay_time - buffer_index) / fade_threshold_time;
    //   *x = (*x) * rate;
    //   // *(x+1) = (*(x+1)) * rate;
    // }
    buffer.write((f32pair_t){*x, *(x+1)});
    buffer_index -= pitch;
    buffer_index += 1.0f;
    if (buffer_index < 0.0f) {
      buffer_index += delay_time;
    } else if (buffer_index >= delay_time) {
      buffer_index -= delay_time;
    }
  }
}


void DELFX_PARAM(uint8_t index, int32_t value) {
  const float valf = q31_to_f32(value);
  switch (index) {
    case 0:
      delay_time = clipminmaxf(0.05f, valf, 1.0f) * BUFFER_SIZE;
      break;
    case 1:
      delay_feedback = clipminmaxf(0.05f, valf, 0.9f);
      break;
    default:
      break;
  }
}

