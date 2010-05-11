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

const bool aabb::intersect(const ray& r, float& tmin) const {
	float tMin = r.tMin, tMax = r.tMax;

	for(unsigned int i=0; i<3; ++i){
		float tMinI = (r.origin(i) - _min(i)) * r.invDir(i);
		float tMaxI = (r.origin(i) - _max(i)) * r.invDir(i);
		if(tMinI > tMaxI){
			swap(tMinI, tMaxI);
		}

		tMin = (tMinI < tMin) ? tMinI : tMin;
		tMax = (tMaxI > tMax) ? tMaxI : tMax;
		if(tMin > tMax){
			return false;
		}
	}

	const __m128 origin = r.origin.getSIMD();
	const __m128 invDir = r.invDir.getSIMD();
	const __m128 boxMin = _min.getSMID();
	const __m128 boxMax = _max.getSMID();

	const __m128 min = mulps(subps(origin, boxMin), invDir);
	const __m128 max = mulps(subps(origin, boxMax), invDir);

	const __m128 min1 = minps(min, max);
	const __m128 max1 = maxps(min, max);

	tmin = tMin;
	return true;
}

/*
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
*/
