/*
Copyright 2019 Tomoaki Itoh
This software is released under the MIT License, see LICENSE.txt.
//*/

#include "userdelfx.h"
#include "buffer_ops.h"
#include "LCWDelay.h"

#define LCW_DELAY_TIME_PARAMS (10)
#define LCW_DELAY_FB_GAIN_TABLE_SIZE (64 + 1)

static __sdram int32_t s_delay_ram_input[LCW_DELAY_INPUT_SIZE];
static __sdram int32_t s_delay_ram_sampling[LCW_DELAY_SAMPLING_SIZE];

static float s_inputGain;
static float s_mix;
static float s_depth;
static uint32_t s_time;

static const float delayTimeParams[LCW_DELAY_TIME_PARAMS] = {
    1.f/8,  // 0.125
    .5f/3 , // 0.1666
    3.f/16, // 0.1875
    // ---------------
    1.f/4 , // 0.25
    1.f/3 , // 0.3333
    3.f/8 , // 0.375
    // ---------------
    1.f/2 , // 0.5
    2.f/3 , // 0.6666
    3.f/4 , // 0.75
    // ---------------
    1.f
};

// u4.28
static const uint32_t fbGainTable[LCW_DELAY_FB_GAIN_TABLE_SIZE] = {
  0x00000000, //[ 0] 0.000000,      off
  0x0066C561, //[ 1] 0.025091,     1.25
  0x00CDF7A8, //[ 2] 0.050285,     1.54
  0x013E09DD, //[ 3] 0.077646,     1.80
  0x01B6A290, //[ 4] 0.107089,     2.06
  0x023688A1, //[ 5] 0.138314,     2.33
  0x02BC3F55, //[ 6] 0.170959,     2.61
  0x03463A4C, //[ 7] 0.204645,     2.90
  0x03D2F744, //[ 8] 0.239005,     3.22
  0x04610CFE, //[ 9] 0.273694,     3.55
  0x04EF33E3, //[10] 0.308399,     3.91
  0x057C4A95, //[11] 0.342845,     4.30
  0x0607579C, //[12] 0.376793,     4.72
  0x068F8901, //[13] 0.410043,     5.17
  0x07143290, //[14] 0.442431,     5.65
  0x0794CB25, //[15] 0.473827,     6.17
  0x0810E976, //[16] 0.504129,     6.72
  0x08884096, //[17] 0.533265,     7.32
  0x08FA9C74, //[18] 0.561184,     7.97
  0x0967DE67, //[19] 0.587859,     8.67
  0x09CFF9F6, //[20] 0.613275,     9.42
  0x0A32F1D7, //[21] 0.637438,    10.23
  0x0A90D53C, //[22] 0.660360,    11.10
  0x0AE9BD6A, //[23] 0.682065,    12.04
  0x0B3DCB9C, //[24] 0.702587,    13.05
  0x0B8D272A, //[25] 0.721961,    14.14
  0x0BD7FBF8, //[26] 0.740231,    15.31
  0x0C1E791E, //[27] 0.757440,    16.58
  0x0C60CFC7, //[28] 0.773636,    17.94
  0x0C9F3245, //[29] 0.788866,    19.42
  0x0CD9D34A, //[30] 0.803180,    21.01
  0x0D10E54A, //[31] 0.816625,    22.73
  0x0D4499FD, //[32] 0.829248,    24.60
  0x0D7521FE, //[33] 0.841097,    26.61
  0x0DA2AC79, //[34] 0.852215,    28.80
  0x0DCD66F6, //[35] 0.862647,    31.17
  0x0DF57D34, //[36] 0.872434,    33.75
  0x0E1B1906, //[37] 0.881616,    36.55
  0x0E3E624D, //[38] 0.890230,    39.61
  0x0E5F7EE8, //[39] 0.898314,    42.94
  0x0E7E92BC, //[40] 0.905902,    46.60
  0x0E9BBFB3, //[41] 0.913025,    50.61
  0x0EB725CE, //[42] 0.919714,    55.02
  0x0ED0E32D, //[43] 0.925998,    59.90
  0x0EE91420, //[44] 0.931904,    65.30
  0x0EFFD33C, //[45] 0.937457,    71.31
  0x0F15396C, //[46] 0.942682,    78.02
  0x0F295E08, //[47] 0.947599,    85.56
  0x0F3C56E2, //[48] 0.952231,    94.08
  0x0F4E3865, //[49] 0.956597,   103.78
  0x0F5F159F, //[50] 0.960714,   114.90
  0x0F6F005B, //[51] 0.964600,   127.77
  0x0F7E0933, //[52] 0.968270,   142.82
  0x0F8C3FA1, //[53] 0.971740,   160.65
  0x0F99B212, //[54] 0.975023,   182.07
  0x0FA66DF6, //[55] 0.978132,   208.28
  0x0FB27FD3, //[56] 0.981079,   241.08
  0x0FBDF34F, //[57] 0.983875,   283.28
  0x0FC8D345, //[58] 0.986530,   339.57
  0x0FD329CD, //[59] 0.989054,   418.39
  0x0FDD004E, //[60] 0.991455,   536.65
  0x0FE65F85, //[61] 0.993743,   733.75
  0x0FEF4F95, //[62] 0.995926,  1127.94
  0x0FF7D810, //[63] 0.998009,  2310.44
  0x10000000  //[64] 1.000000,      inf
};

__fast_inline uint32_t f32_to_u32param(float valf, uint32_t max)
{
  return clipmaxu32( (uint32_t)((valf * max) + 0.5f), max );
}

__fast_inline float lookUpFbGain(float depth)
{
  if ( 1.f <= depth ) {
    return fbGainTable[LCW_DELAY_FB_GAIN_TABLE_SIZE - 1] / (float)(1 << 28);
  }

  uint32_t tmp = (uint32_t)((LCW_DELAY_FB_GAIN_TABLE_SIZE - 1) * depth * 0x100);
  uint32_t i = tmp >> 8;

  // depthが 1.0 に近くなる程、差分の値は小さくなるので uint32_t に収まる
  uint32_t diff = (fbGainTable[i+1] - fbGainTable[i]) * (tmp & 0xFF);
  return (fbGainTable[i] + (diff >> 8)) / (float)(1 << 28);
}

__fast_inline float softlimiter(float c, float x)
{
  float xf = si_fabsf(x);
  if ( xf < c ) {
    return x;
  }
  else {
    return si_copysignf( c + fx_softclipf(c, xf - c), x );
  }
}

void DELFX_INIT(uint32_t platform, uint32_t api)
{
  const LCWDelayNeededBuffer buffer = {
    s_delay_ram_input,
    s_delay_ram_sampling
  };
  
  LCWDelayInit( &buffer );
  LCWDelayReset();

  s_mix = 0.5f;
  s_depth = 0.f;
  s_time = 0;
  s_inputGain = 0.f;
}

void DELFX_PROCESS(float *xn, uint32_t frames)
{
  float bpm = fx_get_bpmf();
  bpm = clampfsel( (float)LCW_DELAY_BPM_MIN, bpm, (float)LCW_DELAY_BPM_MAX );
  float delayTime = delayTimeParams[s_time];
  // bps = bpm / 60
  // n = bps / delayTime
  // delaySamples = samplingRate / n
  //              = (samplingRate * delayTime) / bps
  //              = (samplingRate * delayTime * 60) / bpm
  LCWDelayUpdate( (uint32_t)((LCW_DELAY_SAMPLING_RATE * delayTime * 60.f) / bpm) );

  float * __restrict x = xn;
  const float * x_e = x + 2*frames;

  const float dry = 1.f - s_mix;
  const float wet = s_mix;
  const float fb = lookUpFbGain( s_depth );

  for (; x != x_e; ) {
    float xL = *x;
    float wL = LCWDelayOutput() / (float)(1 << 24);
    wL = softlimiter( 0.1f, wL ); // -> -1.0 .. +1.0

    float inL = (xL * s_inputGain) - (wL * fb);
    LCWDelayInput( (int32_t)(inL * (1 << 24)) );

    float yL = (dry * xL) + (wet * wL);

    *(x++) = yL;
    *(x++) = yL;

    if ( s_inputGain < 0.99998f ) {
      s_inputGain += ( (1.f - s_inputGain) * 0.0625f );
    }
    else { s_inputGain = 1.f; }
  }
}

void DELFX_RESUME(void)
{
  buf_clr_u32(
    (uint32_t * __restrict__)s_delay_ram_input,
    LCW_DELAY_INPUT_SIZE );
  buf_clr_u32(
    (uint32_t * __restrict__)s_delay_ram_sampling,
    LCW_DELAY_SAMPLING_SIZE );
  s_inputGain = 0.f;
}

void DELFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value);
  switch (index) {
  case k_user_delfx_param_time:
    s_time = f32_to_u32param( valf, (LCW_DELAY_TIME_PARAMS - 1) );
    break;
  case k_user_delfx_param_depth:
    s_depth = valf;
    break;
  case k_user_delfx_param_shift_depth:
    // Rescale to add notch around 0.5f
    s_mix = (valf <= 0.49f) ? 1.02040816326530612244f * valf : (valf >= 0.51f) ? 0.5f + 1.02f * (valf-0.51f) : 0.5f;
    break;
  default:
    break;
  }
}
