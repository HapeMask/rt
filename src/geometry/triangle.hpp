#pragma once

#include <cmath>

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "primitive.hpp"
#include "acceleration/intersection.hpp"

#include <tr1/memory>

class triangle : public primitive {
	public:
		triangle(const point3& a, const point3& b, const point3& c);
		virtual ~triangle() {}

		virtual const intersection intersect(ray& r) const;

		virtual const point3& a() const{
			return points[0];
		}

		virtual const point3& b() const{
			return points[1];
		}

		virtual const point3& c() const{
			return points[2];
		}

        virtual const vec3 getNormal(const point3& p) const {
            return normal_;
        }

        inline const vec3& normal() const {
            return normal_;
        }

        virtual void setVertNormals(const vec3& an, const vec3& bn, const vec3& cn);

        virtual const point3 sampleSurface(const float& u0, const float& u1) const;
        inline virtual const float area() const {
            return area_;
        }

	private:
		point3 points[3];
        vec3 vertNormals[3];
        vec2 uvs[3];
		vec3 normal_;
        vec3 B, C;

        float area_;
        bool hasVertNormals, hasUVs;
};

typedef std::tr1::shared_ptr<triangle> trianglePtr;
