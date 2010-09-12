#pragma once

#include "mathlib/constants.hpp"
#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "acceleration/intersection.hpp"
#include "primitive.hpp"

class sphere : public primitive {
	public:
		sphere(const point3& p, const float& r);
		virtual ~sphere() {}

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;

        virtual const point3 sampleSurface(const float& u0, const float& u1) const;
        virtual const vec3 getNormal(const point3& p) const;

        inline virtual const float area() const {
            return 4.f * PI * radius2;
        }

#ifdef RT_USE_QT
        virtual void prepGL(GLfloat*& data) const;
        virtual void drawGL() const;
#endif

        inline virtual const unsigned long vertexCount() const {
            return 0;
        }

	private:
		point3 location;
		float radius, radius2;
};
