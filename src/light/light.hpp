#pragma once

#include <memory>
#include <iostream>

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"

#include "color/color.hpp"
#include "acceleration/intersection.hpp"
#include "materials/material.hpp"

using std::shared_ptr;
using std::cerr;
using std::endl;

class light {
	public:
		light(const point3& p, const float& pow, const rgbColor& c) : position(p), power(pow), lightColor(c) {
            bsdfPtr b(new bsdf());
            b->addBxdf(new lambertianBrdf(rgbColor(1.f)));
            mat = materialPtr(new material(b));
        }

        virtual const rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1,
                float& pdf) const = 0;
        virtual const intersection intersect(const ray& r) const {
            return noIntersect;
        }

        virtual const bool intersectB(const ray& r) const {
            return false;
        }

		inline virtual const rgbColor L(const ray& r) const {
            return lightColor * power;
        }

		inline virtual const rgbColor L(const point3& p) const {
            return (lightColor * power) / norm2(position - p);
        }

        virtual const bool isPointSource() const = 0;

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
        virtual const float pdf(const point3& p) const {
            return 0.f;
        }

        // PDF for generating vector wi with respect to the solid angle
        // subtended by the light at point p.
        virtual const float pdf(const point3& p, const vec3& wi) const {
            return 0.f;
        }

        const materialPtr getMaterial() const {
            return mat;
        }

        virtual const vec3 getNormal(const point3& p) const {
            cerr << "ERROR: light::getNormal() not implemented." << endl;
            exit(1);
        }

        virtual const point3 uniformSampleSurface() const {
            cerr << "ERROR: light::uniformSampleSurface() not implemented." << endl;
            exit(1);
        }

	protected:
        point3 position;
		float power;
		rgbColor lightColor;
        materialPtr mat;
};

typedef shared_ptr<light> lightPtr;
