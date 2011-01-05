#pragma once

#include <cmath>

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "primitive.hpp"
#include "acceleration/intersection.hpp"

#include <tr1/memory>

class triangle : public primitive {
	public:
        triangle() {}

		triangle(const point3& a, const point3& b, const point3& c);
		virtual ~triangle() {}

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;

		const point3& a() const{
			return a_;
		}

		const point3& b() const{
			return b_;
		}

		const point3& c() const{
			return c_;
		}

        virtual const vec3 getNormal(const point3& p) const {
            return normal_;
        }

        inline const vec3& normal() const {
            return normal_;
        }

        void setVertNormals(const vec3& an, const vec3& bn, const vec3& cn);
        void setUVs(const vec2& auv, const vec2& buv, const vec2& cuv);

        virtual const point3 sampleSurface(const float& u0, const float& u1) const;
        inline virtual const float area() const {
            return area_;
        }

#ifdef RT_USE_QT
        virtual void prepGL(GLfloat*& data) const;
        virtual void drawGL() const;
#endif

        inline virtual const long vertexCount() const {
            return 3;
        }

	private:
		point3 a_, b_, c_;
        vec3 vertNormals[3];
        vec2 uvs[3];
		vec3 normal_, binormal_, tangent_;
        vec3 B, C;

        float area_;
        bool hasVertNormals, hasUVs;
};

inline const point3 triangle::sampleSurface(const float& u0, const float& u1) const {
    point3 ret;
    sampleTriangle(ret, *this, u0, u1);
    return ret;
}

inline void triangle::setVertNormals(const vec3& an, const vec3& bn, const vec3& cn){
    vertNormals[0] = an;
    vertNormals[1] = bn;
    vertNormals[2] = cn;
    hasVertNormals = true;
}

inline void triangle::drawGL() const {
    // Do nothing. This triangle's data has been dumped to the scene VBO
    // already and will be drawn as such.
}

typedef std::tr1::shared_ptr<triangle> trianglePtr;
