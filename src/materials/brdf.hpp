#ifndef __RT_BRDF_
#define __RT_BRDF_

#include "color/color.hpp"
#include "mathlib/vector.hpp"
#include <tr1/memory>
using std::tr1::shared_ptr;

class brdf {
	public:
		brdf() : specular(false)
		{}

		brdf(const bool& b) : specular(b)
		{}

		virtual const rgbColor f(const vec3& wi, const vec3& wo) const = 0;
		virtual ~brdf() {}

		const bool& isSpecular(){
			return specular;
		}

	private:
		bool specular;
};

class lambertianBrdf : public brdf {
	public:
		lambertianBrdf(const rgbColor& r);
		virtual ~lambertianBrdf() {}

		virtual const rgbColor f(const vec3& wi, const vec3& wo) const;

	private:
		rgbColor rOverPi;
};

class specularBrdf : public brdf {
	public:
		specularBrdf() : brdf(true)
		{}

		virtual const rgbColor f(const vec3& wi, const vec3& wo) const {
			return rgbColor(0,0,0);
		}

		virtual ~specularBrdf() {}
};

typedef shared_ptr<brdf> brdfPtr;
#endif
