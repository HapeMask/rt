#include "aabb.hpp"
#include "mathlib/sse.hpp"
#include <cmath>

ostream& operator<<(ostream& out, const aabb& b){
	out << "AABB:\n\tTop: " << b.top() <<
		"\n\tBottom: " << b.bottom() <<
		"\n\tLeft: " << b.left() <<
		"\n\tRight: " << b.right() <<
		"\n\tFront: " << b.front() <<
		"\n\tBack: " << b.back() <<
		"\n\tMin: " << b.min() <<
		"\n\tMax: " << b.max();

	return out;
}

const bool aabb::intersect(const ray& r, float& tmin, float& tmax) const {
    const __m128 pos_inf = loadps(PS_POS_INF);
    const __m128 neg_inf = loadps(PS_NEG_INF);

    const __m128 boxMin = _min.getSIMD();
    const __m128 boxMax = _max.getSIMD();
    const __m128 pos = r.origin.getSIMD();
    const __m128 invDir = r.invDir.getSIMD();

    const __m128 l1 = mulps(subps(boxMin, pos), invDir);
    const __m128 l2 = mulps(subps(boxMax, pos), invDir);

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
