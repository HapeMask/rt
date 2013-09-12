#pragma once

#include "mathlib/constants.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "acceleration/intersection.hpp"
#include "primitive.hpp"

class sphere : public primitive {
	public:
		sphere(const point3& p, const float& r);
		virtual ~sphere() {}

		virtual intersection intersect(ray& r) const;
		virtual bool intersectB(const ray& r) const;

        inline virtual point3 sampleSurface(const float& u0, const float& u1) const {
            vec3 v;
            sampleSphere(v, u0, u1);
            return point3(v);
        }

        virtual vec3 getNormal(const point3& p) const;

        inline virtual float area() const {
            return 4.f * PI * radius2;
        }

#ifdef RT_USE_QT
        virtual void prepGL(GLfloat*& vertexData, GLfloat*& normalData) const;
        virtual void drawGL() const;
#endif

        inline virtual long vertexCount() const {
            return 0;
        }

	private:
		point3 location;
		float radius, radius2;
};
