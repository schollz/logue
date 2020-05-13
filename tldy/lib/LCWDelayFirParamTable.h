/*
Copyright 2019 Tomoaki Itoh
This software is released under the MIT License, see LICENSE.txt.
//*/

#include <stdint.h>

#define LCW_DELAY_FIR_TABLE_BITS (8)
#define LCW_DELAY_FIR_TABLE_SIZE (256)
#define LCW_DELAY_FIR_TABLE_MASK (0xFF)
#define LCW_DELAY_FIR_TAP (5)

#define LCW_DELAY_FIR_VALUE_BITS (14)

extern const int16_t gLcwDelayFirTable[][LCW_DELAY_FIR_TAP];

