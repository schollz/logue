/*
Copyright 2019 Tomoaki Itoh
This software is released under the MIT License, see LICENSE.txt.
//*/

#ifndef LCWDelay_h
#define LCWDelay_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define LCW_DELAY_INPUT_BITS (5)
#define LCW_DELAY_INPUT_SIZE (1 << LCW_DELAY_INPUT_BITS)
#define LCW_DELAY_INPUT_MASK (LCW_DELAY_INPUT_SIZE - 1)

#define LCW_DELAY_SAMPLING_BITS (17)
#define LCW_DELAY_SAMPLING_SIZE (1 << LCW_DELAY_SAMPLING_BITS)
#define LCW_DELAY_SAMPLING_MASK (LCW_DELAY_SAMPLING_SIZE - 1)

// サンプリングバッファが2^18もあれば、BPM=30, time=x1.0に対応できる
// 最低でも96k個（= 48000 x 2）は欲しいが、130k個もあるので大丈夫
#define LCW_DELAY_SAMPLING_RATE (48000)
#define LCW_DELAY_BPM_MIN (30)
#define LCW_DELAY_BPM_MAX (480)

typedef struct {
    int32_t *input;
    int32_t *sampling;
} LCWDelayNeededBuffer;

extern void LCWDelayInit(const LCWDelayNeededBuffer *buffer);
extern void LCWDelayReset(void);
extern void LCWDelayUpdate(uint32_t delaySamples);
extern void LCWDelayInput(int32_t fxSend);
extern int32_t LCWDelayOutput(void);

#ifdef __cplusplus
}
#endif

#endif /* LCWDelay_h */
