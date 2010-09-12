#pragma once

#include "samplers/samplers.hpp"
#include "aabb.hpp"
#include "mathlib/point.hpp"
#include "mathlib/ray.hpp"

#include <vector>
#include <tr1/memory>

#ifdef RT_USE_QT
#include <GL/gl.h>
#endif

using namespace std;
using tr1::shared_ptr;

class shape;
typedef shared_ptr<shape> shapePtr;
class intersection;

class primitive {
	public:
        primitive(const aabb& box) : boundingBox(box) {}
        primitive(const aabb& box, shape* p) : parent(p), boundingBox(box) {}

		virtual ~primitive() {}

		virtual const intersection intersect(ray& r) const = 0;
		virtual const bool intersectB(const ray& r) const = 0;

        virtual const point3 sampleSurface(const float& u0, const float& u1) const = 0;
        virtual const point3 uniformSampleSurface() const {
            return sampleSurface(sampleUniform(), sampleUniform());
        }

        virtual const vec3 getNormal(const point3& p) const = 0;
        virtual const float area() const = 0;

		void setParent(shape* p){
            parent = p;
		}

		const aabb& getBounds() const{
			return boundingBox;
		}

		shape* getParent() const {
			return parent;
		}

#ifdef RT_USE_QT
        /**
         * Prepares any data required to draw this shape on an OpenGL
         * framebuffer. Takes an optional pointer to some data as an argument.
         *
         * Data is usually a vertex array.
         */
        virtual void prepGL(GLfloat*& data) const = 0;

        /**
         * Performs the required GL draw calls to draw this shape.
         */
        virtual void drawGL() const = 0;
#endif

        virtual const unsigned long vertexCount() const = 0;

        ray* lastRayTested;

	protected:
		shape* parent;
		aabb boundingBox;
};

typedef shared_ptr<primitive> primitivePtr;
