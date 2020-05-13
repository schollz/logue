#include <cstdint>
#include "userrevfx.h" // これ必要なので注意！

float volume = 0.f; // ボリューム　0.0 ~ 1.0まで

void REVFX_INIT(uint32_t platform, uint32_t api)
{
}


void REVFX_PROCESS(float *xn, uint32_t frames)
{   
  float previousl = 0.f;
  float previousr = 0.f;
  for (auto i = 0u; i < frames; ++i) { // サンプルの数分だけ・・・
    float* l_input = xn++; // L CHのサンプルを取り出す    
    float* r_input = xn++; // R CHのサンプルを取り出す

    float linput = *l_input;
    for( float a = 0.f; a < volume; a = a + 1.0f ) {
      linput = linput * (linput);
   }
   *l_input = *l_input + linput;


    if (*l_input > 1.0) {
      *l_input = 1.0;
    } else if (*l_input < -1.0) {
      *l_input = -1.0;
    }


    float rinput = *r_input;
    for( float a = 0.f; a < volume; a = a + 1.0f ) {
      rinput = rinput * (rinput);
   }
   *r_input = *r_input + rinput;


    if (*r_input > 1.0) {
      *r_input = 1.0;
    } else if (*r_input < -1.0) {
      *r_input = -1.0;
    }

    *l_input = *l_input *0.1;
    *r_input = *r_input *0.1;

  }
}


void REVFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value) *100; // valueを小数の0.0~10.0に変換する
  switch (index) {
    case k_user_revfx_param_time: // A のノブを回したとき・・・
    volume = valf; //　その値をボリューム(0.0~1.1)として使う
    break;
    default:
    break;
  }
}

