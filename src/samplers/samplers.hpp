#ifndef __RT_SAMPLERS__
#define __RT_SAMPLERS__

#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"
#include "mathlib/constants.hpp"

class triangle;
const float sampleUniform();
const int sampleRange(const float& u0, const int& a, const int& b);
void uniformSampleDisk(vec3& v);
void uniformSampleHemisphere(vec3& v);
void sampleTriangle(point3& p, const triangle& t, const float& u0, const float& u1);
void uniformSampleTriangle(point3& p, const triangle& t);
void sampleRectangle(point3& p, const vec3& a, const vec3& b, const point3& pos,
        const float& u0, const float& u1);
void uniformSampleRectangle(point3& p, const vec3& a, const vec3& b, const point3& A);
void cosineSampleHemisphere(vec3& v, const float& u0, const float& u1);
const float radicalInverse(unsigned int n, const int& base);
void getLDSamples2D(float* samples, const unsigned int& count);
const float powerHeuristic(int nf, float fPdf, int ng, float gPdf);
void sampleFirstQuadrant(const float& u0, const float& u1, const float& nu, const float& nv, float& phi, float& costheta);

#endif
