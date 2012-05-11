#pragma once

#include <xmmintrin.h>
#include "constants.hpp"

#define ALIGN_16 __attribute__((aligned(16)))

#define loadps(mem)         _mm_load_ps((const float* const)(mem))
#define storess(ss,mem)     _mm_store_ss((float* const)(mem),(ss))
#define minss               _mm_min_ss
#define minps               _mm_min_ps
#define maxss               _mm_max_ss
#define maxps               _mm_max_ps
#define mulss               _mm_mul_ss
#define sqrtps              _mm_sqrt_ps
#define sqrtss              _mm_sqrt_ss
#define rcpss               _mm_rcp_ss
#define rcpps               _mm_rcp_ps
#define rsqrtps             _mm_rsqrt_ps
#define rsqrtss             _mm_rsqrt_ss
#define shufps              _mm_shuffle_ps
#define shufarg             _MM_SHUFFLE
#define zerops              _mm_setzero_ps
#define set1ps              _mm_set1_ps
#define sse_float2int(f)    _mm_cvtss_si32(_mm_load_ss(&(f)))
#define rotatelps(ps)       _mm_shuffle_ps((ps),(ps), 0x39)
#define muxhps(low,high)    _mm_movehl_ps((low),(high))

#ifdef __SSE4_1__
#include <smmintrin.h>
#define dpps                _mm_dp_ps

// 0x71 = bin(0111 0001), uses lower 3 floats in the input and uses the lowest
// float of the output.
constexpr int DOTMASK_3 = 0x71;
// 0xF1 = bin(1111 0001), uses all 4 floats in the input and uses the lowest
// float of the output.
constexpr int DOTMASK_4 = 0xF1;
#endif

constexpr float PS_POS_INF[4] = {POS_INF, POS_INF, POS_INF, POS_INF};
constexpr float PS_NEG_INF[4] = {NEG_INF, NEG_INF, NEG_INF, NEG_INF};
constexpr float PS_ONES[4] = {1,1,1,1};
constexpr float PS_ZEROES[4] = {0,0,0,0};
