#ifndef __RT_SSE__
#define __RT_SSE__

#include <xmmintrin.h>
#include "constants.hpp"

#define ALIGN_16 __attribute__((aligned(16)))

#define loadps(mem)     _mm_load_ps((const float* const)(mem))
#define storess(ss,mem) _mm_store_ss((float* const)(mem),(ss))
#define minss           _mm_min_ss
#define maxss           _mm_max_ss
#define minps           _mm_min_ps
#define maxps           _mm_max_ps
#define mulps           _mm_mul_ps
#define divps           _mm_div_ps
#define subps           _mm_sub_ps
#define rotatelps(ps)   _mm_shuffle_ps((ps),(ps), 0x39)
#define muxhps(low,high) _mm_movehl_ps((low),(high))

static const float PS_POS_INF[4] = {POS_INF, POS_INF, POS_INF, POS_INF};
static const float PS_NEG_INF[4] = {-POS_INF, -POS_INF, -POS_INF, -POS_INF};

#endif
