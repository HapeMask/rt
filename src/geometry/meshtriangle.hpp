#pragma once

#include "trianglemesh.hpp"

#include "mathlib/point.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "primitive.hpp"
#include "acceleration/intersection.hpp"

#include <cmath>
#include <tr1/memory>

class meshTriangle : public primitive {
	public:
		meshTriangle(const unsigned int& a, const unsigned int& b, const unsigned int& c, triangleMesh* parent);

		virtual const intersection intersect(ray& r) const;
		virtual const bool intersectB(const ray& r) const;

		const unsigned int& aIndex() const{
			return points[0];
		}

		const unsigned int& bIndex() const{
			return points[1];
		}

		const unsigned int& cIndex() const{
			return points[2];
		}

		const point3& a() const{
			return ((triangleMesh*)parent)->pointHeap[points[0]];
		}

		const point3& b() const{
			return ((triangleMesh*)parent)->pointHeap[points[1]];
		}

		const point3& c() const{
			return ((triangleMesh*)parent)->pointHeap[points[2]];
		}

        virtual const vec3 getNormal(const point3& p) const {
            return normal_;
        }

        inline const vec3& normal() const {
            return normal_;
        }

        void setVertNormals(const unsigned int& an, const unsigned int& bn, const unsigned int& cn);
        void setUVs(const unsigned int& auv, const unsigned int& buv, const unsigned int& cuv);

        virtual const point3 sampleSurface(const float& u0, const float& u1) const;
        inline virtual const float area() const {
            return area_;
        }

#ifdef RT_USE_QT
        virtual void prepGL(GLfloat*& data) const;
        virtual void drawGL() const;
#endif

        inline virtual const unsigned long vertexCount() const {
            return 3;
        }

	private:
		unsigned int points[3];
        unsigned int vertNormals[3];
        unsigned int uvs[3];

        float du1,du2,dv1,dv2, invDetUV;

		vec3 normal_, binormal_;
        vec3 B, C;

        float area_;
        bool hasVertNormals, hasUVs;
        triangleMesh* meshParent;
};

typedef std::tr1::shared_ptr<meshTriangle> meshTrianglePtr;
