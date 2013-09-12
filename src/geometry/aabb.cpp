#include "aabb.hpp"
#include "mathlib/sse.hpp"
#include <cmath>

ostream& operator<<(ostream& out, const aabb& b){
    out << "AABB:" <<
        "\n\tMin: " << b.min <<
        "\n\tMax: " << b.max;

    return out;
}

bool aabb::intersect(const ray& r, float& tmin, float& tmax) const {
    const __m128 boxMin = min.xyzw;
    const __m128 boxMax = max.xyzw;
    const __m128 pos = r.origin.xyzw;
    const __m128 invDir = r.invDir.xyzw;

    const __m128 l1 = (boxMin - pos) * invDir;
    const __m128 l2 = (boxMax - pos) * invDir;

    __m128 lmax = maxps(l1, l2);
    __m128 lmin = minps(l1, l2);

    const __m128 lmax0 = rotatelps(lmax);
    const __m128 lmin0 = rotatelps(lmin);
    lmax = minss(lmax, lmax0);
    lmin = maxss(lmin, lmin0);

    const __m128 lmax1 = muxhps(lmax, lmax);
    const __m128 lmin1 = muxhps(lmin, lmin);
    lmax = minss(lmax, lmax1);
    lmin = maxss(lmin, lmin1);

    const bool ret = _mm_comige_ss(lmax, _mm_setzero_ps()) & _mm_comige_ss(lmax, lmin);

    storess(lmin, &tmin);
    storess(lmax, &tmax);

    return ret;
}

bool aabb::intersect(const aabb& box) const {
    const __m128 aMin = min.xyzw;
    const __m128 aMax = max.xyzw;

    const __m128 bMin = box.min.xyzw;
    const __m128 bMax = box.max.xyzw;

    const __m128 test1 = _mm_cmple_ps(aMin, bMax);
    const __m128 test2 = _mm_cmpge_ps(aMax, bMin);

    // Strip off the 4th component since it'll always be -nan.
    const int res1 = _mm_movemask_ps(test1) & 0x7;
    const int res2 = _mm_movemask_ps(test2) & 0x7;

    return (res1 && res2);
}
