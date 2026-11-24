/*
 * Charybdis (58-key standard) key labels
 * Based on matrix_transform0 (full 5×12 matrix)
 *
 * Follows https://github.com/urob/zmk-helpers/blob/main/docs/key_labels.md#standardization
 *
 * Layout:
 *   ╭───────────────────────────┬───────────────────────────╮
 *   │ LN5 LN4 LN3 LN2 LN1 LN0  │ RN0 RN1 RN2 RN3 RN4 RN5  │   ← number row
 *   │ LT5 LT4 LT3 LT2 LT1 LT0  │ RT0 RT1 RT2 RT3 RT4 RT5  │   ← top row
 *   │ LM5 LM4 LM3 LM2 LM1 LM0  │ RM0 RM1 RM2 RM3 RM4 RM5  │   ← middle row
 *   │ LB5 LB4 LB3 LB2 LB1 LB0  │ RB0 RB1 RB2 RB3 RB4 RB5  │   ← bottom row
 *   │        LH4 LH3 LH2 LH1 LH0 │ RH0 RH1 RH2 RH3 RH4     │   ← thumb row (5 each side)
 *   ╰───────────────────────────┴───────────────────────────╯
 *
 * Values correspond to matrix positions (0–58) in matrix_transform0.
 */

#pragma once

/* Left number row */
#define LN0  5
#define LN1  4
#define LN2  3
#define LN3  2
#define LN4  1
#define LN5  0

/* Right number row */
#define RN0  6
#define RN1  7
#define RN2  8
#define RN3  9
#define RN4 10
#define RN5 11

/* Left top row */
#define LT0 17
#define LT1 16
#define LT2 15
#define LT3 14
#define LT4 13
#define LT5 12

/* Right top row */
#define RT0 18
#define RT1 19
#define RT2 20
#define RT3 21
#define RT4 22
#define RT5 23

/* Left middle row */
#define LM0 29
#define LM1 28
#define LM2 27
#define LM3 26
#define LM4 25
#define LM5 24

/* Right middle row */
#define RM0 30
#define RM1 31
#define RM2 32
#define RM3 33
#define RM4 34
#define RM5 35

/* Left bottom row */
#define LB0 41
#define LB1 40
#define LB2 39
#define LB3 38
#define LB4 37
#define LB5 36

/* Right bottom row */
#define RB0 42
#define RB1 43
#define RB2 44
#define RB3 45
#define RB4 46
#define RB5 47

/* Left thumb row (inner → outer) */
#define LH0 53   // RC(4,0)
#define LH1 52   // RC(4,1)
#define LH2 51   // RC(4,2)
#define LH3 50   // RC(4,3)
#define LH4 49   // RC(4,4)

/* Right thumb row (inner → outer) */
#define RH0 54   // RC(4,6)
#define RH1 55   // RC(4,7)
#define RH2 56   // RC(4,8)
#define RH3 57   // RC(4,9)
#define RH4 58   // RC(4,10)

/* Group macros for convenience */
#define NUMROW    LN0 LN1 LN2 LN3 LN4 LN5 RN0 RN1 RN2 RN3 RN4 RN5
#define KEYS_L    LT0 LT1 LT2 LT3 LT4 LT5 LM0 LM1 LM2 LM3 LM4 LM5 LB0 LB1 LB2 LB3 LB4 LB5
#define KEYS_R    RT0 RT1 RT2 RT3 RT4 RT5 RM0 RM1 RM2 RM3 RM4 RM5 RB0 RB1 RB2 RB3 RB4 RB5
#define THUMBS_L  LH0 LH1 LH2 LH3 LH4
#define THUMBS_R  RH0 RH1 RH2 RH3 RH4
#define THUMBS    THUMBS_L THUMBS_R
