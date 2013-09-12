#pragma once

#include <iostream>

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"

#include "color/color.hpp"
#include "acceleration/intersection.hpp"
#include "materials/material.hpp"

class light {
	public:
		light(const point3& p, const float& pow, const rgbColor& c) : position(p), power(pow), lightColor(c) {
            bsdf* b = new bsdf();
            b->addBxdf(new lambertianBrdf(rgbColor(1.f)));
            mat = materialPtr(new material(b));
        }

        virtual rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1,
                float& pdf) const = 0;
        virtual intersection intersect(const ray& r) const {
            return noIntersect;
        }

        virtual bool intersectB(const ray& r) const {
            return false;
        }

		inline virtual rgbColor L(const ray& r) const {
            return lightColor * power;
        }

		inline virtual rgbColor L(const point3& p) const {
            return (lightColor * power) / norm2(position - p);
        }

        virtual bool isPointSource() const = 0;

        const point3& getPosition() const {
            return position;
        }

		const float& getPower() const {
			return power;
		}

		const rgbColor& getColor() const {
			return lightColor;
		}

        // PDF of for generating point p on the surface of the light.
        virtual float pdf(const point3& p) const {
            return 0.f;
        }

        // PDF for generating vector wi with respect to the solid angle
        // subtended by the light at point p.
        virtual float pdf(const point3& p, const vec3& wi) const {
            return 0.f;
        }

        const materialPtr getMaterial() const {
            return mat;
        }

        virtual vec3 getNormal(const point3& p) const = 0;
        virtual point3 uniformSampleSurface() const = 0;

	protected:
        point3 position;
		float power;
		rgbColor lightColor;
        materialPtr mat;
};
