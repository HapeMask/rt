#include "samplers.hpp"
#include "mathlib/SFMT.h"

#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"
#include "mathlib/constants.hpp"
#include "geometry/triangle.hpp"

#include <cstdlib>
#include <cmath>

const float sampleUniform(){
	//return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return genrand_real1();
}

const int sampleRange(const unsigned int& a, const unsigned int& b){
    return a + (gen_rand32() % (b-a+1));
}

void sampleDisk(vec3& v, const float& u0, const float& u1){
	const float r = sqrtf(u0);
	const float theta = u1 * TWOPI;
	v.x() = cos(theta) * r;
	v.z() = sin(theta) * r;
}

void uniformSampleHemisphere(vec3& v){
    const float u0 = sampleUniform();
    const float u1 = sampleUniform();

	v.y() = u0;
	const float r = sqrtf(max(0.f, 1.f - u0*u0));
	const float phi = TWOPI * u1;
	v.x() = r * cos(phi);
	v.z() = r * sin(phi);
}

void sampleTriangle(point3& p, const triangle& t, const float& u0, const float& u1){
    const float su0 = sqrtf(u0);
    const float u = 1.f - su0;
    const float v = u1 * su0;

    p = (u * t.a() + v * t.b() + (1.f - u - v) * t.c());
}

void uniformSampleTriangle(point3& p, const triangle& t){
    sampleTriangle(p, t, sampleUniform(), sampleUniform());
}

void sampleRectangle(point3& p, const vec3& a, const vec3& b, const point3& pos,
        const float& u0, const float& u1){
    p = pos + (0.5f-u0) * a + (0.5f-u1) * b;
}

void uniformSampleRectangle(point3& p, const vec3& a, const vec3& b, const point3& pos){
    sampleRectangle(p, a, b, pos, sampleUniform(), sampleUniform());
}

void cosineSampleHemisphere(vec3& v, const float& u0, const float& u1){
	sampleDisk(v, u0, u1);
    v.y() = sqrtf(max(0.f, 1.f - v.x()*v.x() - v.z()*v.z()));
}

const float radicalInverse(unsigned int n, const int& base){
    float val = 0.f;
    const double invBase = 1.f / base;
    float invBi = invBase;
    while(n > 0){
        val += (n % base) * invBi;
        n /= base;
        invBi *= invBase;
    }

    return val;
}

void getLDSamples2D(float* samples, const unsigned int& count){
    const float invN = 1.f / (float)count;

    for(unsigned int i=0; i<count; ++i){
        samples[2*i] = (float)i * invN;
        samples[2*i+1] = radicalInverse(i, primes[0]);
    }
}

const float powerHeuristic(int nf, float fPdf, int ng, float gPdf){
    const float f = nf*fPdf;
    const float g = ng*gPdf;
    return (f*f) / (f*f + g*g);
}
