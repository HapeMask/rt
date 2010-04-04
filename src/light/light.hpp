#ifndef __RT_LIGHT__
#define __RT_LIGHT__

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"

class light {
	public:
		virtual const rgbColor L(const vec3& wo) const = 0;
		light(const point3& p) : position(p) {}

		const point3& getPosition(){
			return position;
		}

	protected:
		point3 position;
};

class pointLight : public light {
	public:
		pointLight(const point3& p);
		pointLight(const point3& p, const rgbColor& c);

		virtual const rgbColor L(const vec3& wo) const;

		const rgbColor& getColor() const{
			return _color;
		}

	private:
		rgbColor _color;
};
#endif
