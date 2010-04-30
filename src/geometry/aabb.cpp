#include "aabb.hpp"
#include "mathlib/sse.hpp"

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

/*
 * http://www.flipcode.com/archives/SSE_RayBox_Intersection_Test.shtml
 */
const bool aabb::intersect(const ray& r, float& tmin) const {
    //const __m128 pos_inf = loadps(PS_POS_INF);
    //const __m128 neg_inf = loadps(PS_NEG_INF);
	__m128 pos_inf, neg_inf;
	memcpy(&pos_inf, PS_POS_INF, 16);
	memcpy(&neg_inf, PS_NEG_INF, 16);

    const __m128 boxMin = _min.getSIMD();
    const __m128 boxMax = _max.getSIMD();
    const __m128 pos = r.origin.getSIMD();
    const __m128 dir = r.direction.getSIMD();

    const __m128 l1 = divps(subps(boxMin, pos), dir);
    const __m128 l2 = divps(subps(boxMax, pos), dir);

    // Remove NaNs.
    const __m128 filt_l1a = minps(l1, pos_inf);
    const __m128 filt_l2a = minps(l2, pos_inf);

    const __m128 filt_l1b = maxps(l1, neg_inf);
    const __m128 filt_l2b = maxps(l2, neg_inf);

    __m128 lmax = maxps(filt_l1a, filt_l2a);
    __m128 lmin = minps(filt_l1b, filt_l2b);

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
    //storess(lmax, &tmax);

    return ret;
}
