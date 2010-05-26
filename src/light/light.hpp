#ifndef __RT_LIGHT__
#define __RT_LIGHT__

#include <tr1/memory>

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "color/color.hpp"
#include "acceleration/intersection.hpp"

using namespace std;
using tr1::shared_ptr;

class light {
	public:
		light(const point3& p, const float& pow, const rgbColor& c) : position(p), power(pow), lightColor(c) {}

        virtual const rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pdf) const = 0;
        virtual const intersection intersect(const ray& r) const {
            return noIntersect;
        }

		inline virtual const rgbColor L(const point3& p) const {
            return lightColor * power * (1.f / (position - p).length2());
        }

        virtual const bool isPointSource() const = 0;

        const point3& getPosition() const{
            return position;
        }

		const float& getPower(){
			return power;
		}

		const rgbColor& getColor() const{
			return lightColor;
		}

        virtual const float pdf() const{
            return 0.f;
        }

	protected:
        point3 position;
		rgbColor lightColor;
		float power;
};

class pointLight : public light {
	public:
		pointLight(const point3& p, const float& pow, const rgbColor& c);

        inline virtual const bool isPointSource() const {
            return true;
        }

        virtual const rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pdf) const;
        inline virtual const float pdf() const{
            return 0.f;
        }
};

class areaLight : public light {
    public:
        areaLight(const point3& p, const float& pow, const rgbColor& c,
                const vec3& vA, const vec3& vB);

        virtual const intersection intersect(const ray& r) const;

        inline virtual const bool isPointSource() const {
            return false;
        }

        inline virtual const float pdf() const {
            return invArea;
        }

        virtual const rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pdf) const;
		virtual const rgbColor L(const point3& p) const;

    private:
        vec3 a, b, normal;
        vec3 A, B, C, D;
        float invArea;
};

typedef shared_ptr<light> lightPtr;
#endif
