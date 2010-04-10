#ifndef __RT_LIGHT__
#define __RT_LIGHT__

#include <tr1/memory>

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "color/color.hpp"

using namespace std;
using tr1::shared_ptr;

class light {
	public:
		light(const point3& p, const float& pow) : position(p), power(pow) {}

		virtual const rgbColor L(const point3& p) const = 0;

		const point3& getPosition(){
			return position;
		}

		const float& getPower(){
			return power;
		}

	protected:
		point3 position;
		float power;
};

class pointLight : public light {
	public:
		pointLight(const point3& p, const rgbColor& c, const float& pow);

		virtual const rgbColor L(const point3& p) const;

		const rgbColor& getColor() const{
			return lightColor;
		}

	private:
		rgbColor lightColor;
};

typedef shared_ptr<light> lightPtr;
#endif
