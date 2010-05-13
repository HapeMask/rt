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
		light(const point3& p, const float& pow, const rgbColor& c) : lightColor(c), position(p), power(pow) {}

        virtual const rgbColor sampleL(const float& u0, const float& u1) const = 0;

		virtual const rgbColor L(const point3& p) const = 0;
        virtual const bool isPointSource() const = 0;

        const point3& getPosition(){
            return position;
        }

		const float& getPower(){
			return power;
		}

		const rgbColor& getColor() const{
			return lightColor;
		}

	protected:
        vec3 position;
		rgbColor lightColor;
		float power;
};

class pointLight : public light {
	public:
		pointLight(const point3& p, const rgbColor& c, const float& pow);
        inline virtual const bool isPointSource() const {
            return true;
        }

        virtual const rgbColor sampleL(const float& u0, const float& u1) const {
            return 0.f;
        }
		virtual const rgbColor L(const point3& p) const;
};

class areaLight : public light {
    public:
        areaLight::areaLight(const point3& p, const float& pow, const rgbColor& c,
                const vec3& A, const vec3& B,
                const vec3& normal, const float& w, const float& h);
        inline virtual const bool isPointSource() const {
            return false;
        }

        virtual const rgbColor sampleL(const float& u0, const float& u1) const;
		virtual const rgbColor L(const point3& p) const;

    private:
        vec3 a, b, normal;
        float width, height;
};

typedef shared_ptr<light> lightPtr;
#endif
