#pragma once

#include "mathlib/vector.hpp"
#include "color/color.hpp"
#include "acceleration/intersection.hpp"
#include "mathlib/sse.hpp"

inline constexpr float radians(const float& deg){
	return (deg / 180.f) * PI;
}

inline constexpr float degrees(const float& rad){
	return (rad / PI) * 180.f;
}

inline constexpr bool isFinite(const float& f){
    return (f >= -MAX_FLOAT && f <= MAX_FLOAT);
}

inline rgbColor clamp(const rgbColor& c){
    if(!(isFinite(c.red()) && isFinite(c.green()) && isFinite(c.blue()))){
        return rgbColor(0.f);
    }else{
        const float r = std::min(std::max(c.red(), 0.f), 1.f);
        const float g = std::min(std::max(c.green(), 0.f), 1.f);
        const float b = std::min(std::max(c.blue(), 0.f), 1.f);
        return rgbColor(r,g,b);
    }
}

inline vec3 clamp(const vec3& v){
    return vec3(min(max(v, vec3(0.f)), vec3(1.f)));
}

inline float clamp(const float& f){
	return std::min(std::max(f, 0.f), 1.f);
}


inline vec3 reflect(const vec3& v, const vec3& n){
	return v - (2.f * dot(v,n)*n);
}

void makeCoordinateSystem(const vec3& u, vec3& v, vec3& w);


inline vec3 worldToBsdf(const vec3& v, const intersection& isect) {
    return vec3(dot(v, isect.binormal), dot(v, isect.shadingNormal), dot(v, isect.tangent));
}


inline vec3 bsdfToWorld(const vec3& v, const intersection& isect){
    return vec3(
        isect.binormal.x * v.x + isect.shadingNormal.x * v.y + isect.tangent.x * v.z,
        isect.binormal.y * v.x + isect.shadingNormal.y * v.y + isect.tangent.y * v.z,
        isect.binormal.z * v.x + isect.shadingNormal.z * v.y + isect.tangent.z * v.z);
}

inline vec3 worldToBsdf(const vec3& v, const vec3& normal, const vec3& binormal, const vec3& tangent){
    return vec3(dot(v, binormal), dot(v, normal), dot(v, tangent));
}

inline vec3 bsdfToWorld(const vec3& v, const vec3& normal, const vec3& binormal, const vec3& tangent){
    return vec3(
        binormal.x * v.x + normal.x * v.y + tangent.x * v.z,
        binormal.y * v.x + normal.y * v.y + tangent.y * v.z,
        binormal.z * v.x + normal.z * v.y + tangent.z * v.z);
}

inline void sphericalToDirection(vec3& v, const float& sinTheta, const float& cosTheta, const float& phi){
    v.x = sinTheta * cosf(phi);
    v.y = cosTheta;
    v.z = sinTheta * sinf(phi);
}

/**
 * Computes the double integral of the 2D Gaussian with stddev sigma for X and
 * Y in the range [-0.5, 0.5]
 */
inline constexpr float gaussian2DNormalization(const float& sigma) {
    return 0.3989422804f * fabsf(1.f/sigma) * erf(0.353553389f *
            fabsf(1.f/sigma)) * (-1.25331413f * sigma *
            erf(-0.353553389f/sigma) + 1.25331413f * sigma *
            erf(0.353553389f/sigma));
}

/**
 * Fast Float/Int conversion from PBRT.
 */
//2^52 * 1.5, uses limited precision to floor.
constexpr double doublemagic = 6755399441055744.0;
constexpr double doublemagicroundeps = .5-1.4e-11;

/**
 * Fast Float/Int conversion from PBRT.
 */
inline int Round2Int(double val) {
	val	= val + doublemagic;
	return (reinterpret_cast<long*>(&val))[0];
}

inline int Float2Int(const float val) {
    return sse_float2int(val);
}

inline int Floor2Int(const double val) {
	return Round2Int(val - doublemagicroundeps);
}

inline int Ceil2Int(const double val) {
	return Round2Int(val + doublemagicroundeps);
}
