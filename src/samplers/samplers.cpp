#include "samplers.hpp"
#include "utility.hpp"
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

const int sampleRange(const float& u0, const int& a, const int& b){
    return a + min((u0*(b-a+1)), (float)b-a);
}

void sampleDisk(vec3& v, const float& u0, const float& u1){
	const float r = sqrtf(u0);
	const float theta = u1 * TWOPI;
	v.x() = cos(theta) * r;
	v.z() = sin(theta) * r;
}

void sampleHemisphere(vec3& v, const float& u0, const float& u1) {
	v.y() = u0;
    // Sqrt of r normalizes the distance distribution from the center (since it
    // takes up ~r^2 less area as radius decreases).
	const float r = sqrtf(max(0.f, 1.f - u0*u0));
	const float phi = TWOPI * u1;
	v.x() = r * cos(phi);
	v.z() = r * sin(phi);
}

void uniformSampleHemisphere(vec3& v) {
    sampleHemisphere(v, sampleUniform(), sampleUniform());
}

void sampleSphere(vec3& v, const float& u0, const float& u1) {
    // Sample the hemisphere then flip with probability 1/2 to uniformly sample
    // the sphere. Need to rescale u0 to avoid correlation between
    // hemispheres/samples.
    if(u0 < 0.5f){
        sampleHemisphere(v, 2.f * u0, u1);
    }else{
        sampleHemisphere(v, 2.f * (1 - u0), u1);
        v.y() = -v.y();
    }
}

void uniformSampleSphere(vec3& v) {
    sampleSphere(v, sampleUniform(), sampleUniform());
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

const float radicalInverse(int n, const int& base){
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

void getLDSamples2D(float* samples, const int& count){
    const float invN = 1.f / (float)count;

    for(int i=0; i<count; ++i){
        samples[2*i] = (float)i * invN;
        samples[2*i+1] = radicalInverse(i, primes[0]);
    }
}

void getLDSamples3D(float* samples, const int& count){
    const float invN = 1.f / (float)count;

    for(int i=0; i<count; ++i){
        samples[3*i] = (float)i * invN;
        samples[3*i+1] = radicalInverse(i, primes[0]);
        samples[3*i+2] = radicalInverse(i, primes[1]);
    }
}

const float powerHeuristic(int nf, float fPdf, int ng, float gPdf){
    const float f = nf*fPdf;
    const float g = ng*gPdf;
    return (f*f) / (f*f + g*g);
}

const float balanceHeuristic(int nf, float fPdf, int ng, float gPdf){
    return (nf * fPdf) / (nf * fPdf + ng * gPdf);
}

void sampleFirstQuadrant(const float& u0, const float& u1, const float& nu, const float& nv, float& phi, float& costheta){
    if (nu == nv){
        phi = PI * u0 * 0.5f;
    }else{
        phi = atanf(sqrtf((nu+1)/(nv+1)) *
            tanf(PI * u0 * 0.5f));
    }

    const float cosphi = cosf(phi), sinphi = sinf(phi);
    costheta = powf(u1, 1.f/(nu * cosphi * cosphi +
        nv * sinphi * sinphi + 1.f));
}
