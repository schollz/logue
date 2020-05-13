#include <cstdint>
#include "userrevfx.h" // これ必要なので注意！

float volume = 0.f; // ボリューム　0.0 ~ 1.0まで

void REVFX_INIT(uint32_t platform, uint32_t api)
{
}


void REVFX_PROCESS(float *xn, uint32_t frames)
{   
  for (auto i = 0u; i < frames; ++i) { // サンプルの数分だけ・・・
    float* l_input = xn++; // L CHのサンプルを取り出す    
    float* r_input = xn++; // R CHのサンプルを取り出す

    *l_input = *l_input * volume; // L CHのサンプルはL CHにボリュームを掛けたもの（デカい)
    
    // デカい場合は+-1.0にする
    if (*l_input > 1.0) {
      *l_input = 1.0;
    } else if (*l_input < -1.0) {
      *l_input = -1.0;
    }


    // RCHも同じ
    *r_input = *r_input * volume; 
    
    if (*r_input > 1.0) {
      *r_input = 1.0;
    } else if (*r_input < -1.0) {
      *r_input = -1.0;
    }

    *l_input = *l_input *0.5;
    *r_input = *r_input *0.5;
    
  }
}


void REVFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value) * 10000; // valueを小数の0.0~10.0に変換する
  switch (index) {
    case k_user_revfx_param_time: // A のノブを回したとき・・・
    volume = valf; //　その値をボリューム(0.0~1.1)として使う
    break;
    default:
    break;
  }
}

