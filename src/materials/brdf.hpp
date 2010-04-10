#ifndef __RT_BRDF_
#define __RT_BRDF_

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include <tr1/memory>
using std::tr1::shared_ptr;

class brdf {
	public:
		virtual const rgbColor f(const vec3& wi, const vec3& wo) const = 0;
		virtual ~brdf() {}
};

class lambertianBrdf : public brdf {
	public:
		lambertianBrdf(const rgbColor& r);
		virtual ~lambertianBrdf() {}

		virtual const rgbColor f(const vec3& wi, const vec3& wo) const;

	private:
		rgbColor rOverPi;
};

typedef shared_ptr<brdf> brdfPtr;
#endif
