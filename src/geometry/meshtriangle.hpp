#pragma once
#include <memory>
#include <cmath>

#include "trianglemesh.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "primitive.hpp"
#include "acceleration/intersection.hpp"

class meshTriangle : public primitive {
	public:
		meshTriangle(const int& a, const int& b, const int& c, triangleMesh* parent);

		virtual const intersection intersect(ray& r) const;
		virtual bool intersectB(const ray& r) const;

		const int& aIndex() const{
			return points[0];
		}

		const int& bIndex() const{
			return points[1];
		}

		const int& cIndex() const{
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

        void setVertNormals(const int& an, const int& bn, const int& cn);
        void setUVs(const int& auv, const int& buv, const int& cuv);

        virtual const point3 sampleSurface(const float& u0, const float& u1) const;
        inline virtual float area() const {
            return area_;
        }

#ifdef RT_USE_QT
        virtual void prepGL(GLfloat*& vertexData, GLfloat*& normalData) const;
        virtual void drawGL() const;
#endif

        inline virtual long vertexCount() const {
            return 3;
        }

	private:
		int points[3];
        int vertNormals[3];
        int uvs[3];

        float du1,du2,dv1,dv2, invDetUV;

		vec3 normal_, binormal_;
        vec3 B, C;

        float area_;
        bool hasVertNormals, hasUVs;
        triangleMesh* meshParent;
};

typedef std::shared_ptr<meshTriangle> meshTrianglePtr;
