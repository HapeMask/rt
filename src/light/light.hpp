#pragma once

#include <tr1/memory>

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "color/color.hpp"
#include "acceleration/intersection.hpp"
#include "materials/material.hpp"

using namespace std;
using tr1::shared_ptr;

class light {
	public:
		light(const point3& p, const float& pow, const rgbColor& c) : position(p), power(pow), lightColor(c) {
            bsdfPtr b(new bsdf());
            b->addBxdf(new lambertianBrdf(1.f));
            mat = materialPtr(new material(b));
        }

        virtual const rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pdf) const = 0;
        virtual const intersection intersect(const ray& r) const {
            return noIntersect;
        }

		inline virtual const rgbColor L(const ray& r) const {
            return lightColor * power * (1.f / (position - r.origin).length2());
        }

		inline virtual const rgbColor L(const point3& p) const {
            return lightColor * power * (1.f / (position - p).length2());
        }

        virtual const bool isPointSource() const = 0;

        const point3& getPosition() const {
            return position;
        }

		const float& getPower(){
			return power;
		}

		const rgbColor& getColor() const {
			return lightColor;
		}

        virtual const float pdf(const point3& p) const {
            return 0.f;
        }

        virtual const float pdf(const point3& p, const vec3& wi) const {
            return 0.f;
        }

        const materialPtr getMaterial() const {
            return mat;
        }

        virtual const vec3& getNormal() const {
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

class pointLight : public light {
	public:
		pointLight(const point3& p, const float& pow, const rgbColor& c);

        inline virtual const bool isPointSource() const {
            return true;
        }

        virtual const rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pdf) const;
};

class areaLight : public light {
    public:
        areaLight(const point3& p, const float& pow, const rgbColor& c,
                const vec3& vA, const vec3& vB);

        virtual const intersection intersect(const ray& r) const;

        inline virtual const bool isPointSource() const {
            return false;
        }

        inline virtual const float pdf(const point3& p) const {
            return invArea;
        }

        virtual const float pdf(const point3& p, const vec3& wi) const;

        virtual const rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pd) const;
		virtual const rgbColor L(const ray& r) const;

        virtual const vec3& getNormal() const {
            return normal;
        }

        inline virtual const point3 uniformSampleSurface() const {
            point3 samplePoint;
            sampleRectangle(samplePoint, a, b, position, sampleUniform(), sampleUniform());
            return samplePoint;
        }

    private:
        vec3 a, b, normal;
        vec3 A, B, C, D;
        const float area, invArea;
};

typedef shared_ptr<light> lightPtr;
