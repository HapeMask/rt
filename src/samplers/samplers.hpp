#pragma once
#include <random>

#include "mathlib/vector.hpp"
#include "mathlib/constants.hpp"

class triangle;

// TODO: FIX THIS?
static std::mt19937 rng;
static std::uniform_real_distribution<float> uniform;

inline float sampleUniform(){ return uniform(rng); }

int sampleRange(const float& u0, const int& a, const int& b);

void uniformSampleDisk(vec3& v);

void sampleHemisphere(vec3& v, const float& u0, const float& u1);
void uniformSampleHemisphere(vec3& v);
void cosineSampleHemisphere(vec3& v, const float& u0, const float& u1);

void sampleSphere(vec3& v, const float& u0, const float& u1);
void uniformSampleSphere(vec3& v);

void sampleTriangle(point3& p, const triangle& t, const float& u0, const float& u1);
void uniformSampleTriangle(point3& p, const triangle& t);
void sampleRectangle(point3& p, const vec3& a, const vec3& b, const point3& pos,
        const float& u0, const float& u1);
void uniformSampleRectangle(point3& p, const vec3& a, const vec3& b, const point3& A);
float radicalInverse(int n, const int& base);
void getLDSamples2D(float* samples, const int& count);
void getLDSamples3D(float* samples, const int& count);
float powerHeuristic(int nf, float fPdf, int ng, float gPdf);
float balanceHeuristic(int nf, float fPdf, int ng, float gPdf);
void sampleFirstQuadrant(const float& u0, const float& u1, const float& nu, const float& nv, float& phi, float& costheta);

float evaluate2DGaussian(const float& x, const float& y, const float& sigma);
