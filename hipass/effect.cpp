#include <cstdint>
#include "userrevfx.h" // これ必要なので注意！

// 前の信号を覚えておくメモリ（バッファ）を作る
#define BUFFER_SIZE (10) // バッファの長さ 50個くらいまではうまくいく。あまり大きいと破綻
float lch_buffer[BUFFER_SIZE]; 
float rch_buffer[BUFFER_SIZE];

int num_samples_to_use = 1; 

void REVFX_INIT(uint32_t platform, uint32_t api)
{
  // バッファを全部0にする
  for (auto i = 0; i < BUFFER_SIZE; i++) {
    lch_buffer[i] = 0.0;
    rch_buffer[i] = 0.0;
  }
}


void REVFX_PROCESS(float *xn, uint32_t frames)
{   

  // 平均を求める関数
  auto movingAverage = [](float input, float* buffer, const int n) -> float {
    // バッファを全部ズラす // 実際はこうしないで、リングバッファというのを使う
    for (auto i = 0; i < n-1; ++i) {
      buffer[i] = buffer[i+1];
    }
    // 最後に今のサンプルを入れる
    buffer[n-1] = input;

    // 平均値を作る
    float output = 0;
    for (auto i = 0; i < n; ++i) {
      output += buffer[i];
    }

    return output / n;
  };

  for (auto i = 0u; i < frames; ++i) { // サンプルの数分だけ・・・
    float* l_input = xn++; // L CHのサンプルを取り出す    
    float* r_input = xn++; // R CHのサンプルを取り出す

    const float l_filtered = movingAverage(*l_input, lch_buffer, num_samples_to_use);
    const float r_filtered = movingAverage(*r_input, rch_buffer, num_samples_to_use);

    *l_input = *l_input - l_filtered; // LCH ローパスフィルタしたものを引く =　高い信号が残る
    *r_input = *r_input - r_filtered; // RCH ローパスフィルタしたものを引く =　高い信号が残る

  }

}


void REVFX_PARAM(uint8_t index, int32_t value)
{
  int val = q31_to_f32(value) * BUFFER_SIZE; // valueを整数の 1 - 50に変換する
  if (val == 0) {
    val = 1;
  } else if (val >= BUFFER_SIZE) {
    val = BUFFER_SIZE;
  }

  switch (index) {
    case k_user_revfx_param_time: // A のノブを回したとき・・・
    num_samples_to_use = val; //　その値を平均する数として使う
    break;
    default:
    break;
  }
}

