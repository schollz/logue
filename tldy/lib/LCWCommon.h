/*
Copyright 2019 Tomoaki Itoh
This software is released under the MIT License, see LICENSE.txt.
//*/

#ifndef LCWCommon_h
#define LCWCommon_h

#include <stdint.h>

typedef int16_t SQ3_12;
typedef uint16_t UQ4_12;
typedef int16_t SQ1_14;
typedef uint16_t UQ2_14;
typedef int32_t SQ15_16;
typedef uint32_t UQ16_16;
typedef int32_t SQ7_24;
typedef uint32_t UQ8_24;
typedef int32_t SQ3_28;
typedef uint32_t UQ4_28;
typedef int64_t SQ39_24;
typedef uint64_t UQ40_24;

#define LCW_SQ3_12(val)  (  (SQ3_12)((val) *  (int16_t)0x1000))
#define LCW_UQ4_12(val)  (  (UQ4_12)((val) * (uint16_t)0x1000))
#define LCW_SQ1_14(val)  (  (SQ1_14)((val) *  (int16_t)0x4000))
#define LCW_UQ2_14(val)  (  (UQ2_14)((val) * (uint16_t)0x4000))
#define LCW_SQ15_16(val) ( (SQ15_16)((val) *  (int32_t)0x10000))
#define LCW_UQ16_16(val) ( (UQ16_16)((val) * (uint32_t)0x10000))
#define LCW_SQ7_24(val)  (  (SQ7_24)((val) *  (int32_t)0x1000000))
#define LCW_UQ8_24(val)  (  (UQ8_24)((val) * (uint32_t)0x1000000))
#define LCW_SQ3_28(val)  (  (SQ3_28)((val) *  (int32_t)0x10000000))
#define LCW_UQ4_28(val)  (  (UQ4_28)((val) * (uint32_t)0x10000000))
#define LCW_SQ39_24(val) ( (SQ39_24)((val) *  (int64_t)0x1000000))
#define LCW_UQ40_24(val) ( (UQ40_24)((val) * (uint64_t)0x1000000))

#define LCW_CLIP(val,min,max) ( ((val) < (min)) ? (min) : (((max) < (val)) ? (max) : (val)) )
#define LCW_ROUND(val) ( ( val < .0 ) ? (val - .5) : (val + .5) )
#define LCW_ABS(val) ( ((val) < 0) ? -(val) : (val) )

#endif /* LCWCommon_h */
