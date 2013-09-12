#pragma once
#include <memory>
#include <vector>

#ifdef RT_USE_QT
#include <GL/glew.h>
#endif

#include "samplers/samplers.hpp"
#include "aabb.hpp"
#include "mathlib/ray.hpp"

using std::shared_ptr;
using std::vector;

class shape;
class intersection;

class primitive {
	public:
        primitive() : lastRayTested(nullptr), parent(nullptr) {}
        primitive(const aabb& box) : primitive(nullptr, nullptr, box) {}
        primitive(const aabb& box, shape* p) : primitive(nullptr, p, box) {}
        primitive(ray* lrt, shape* p, const aabb& box) :
            lastRayTested(lrt), parent(p), boundingBox(box) {}

		virtual ~primitive() {}

		virtual intersection intersect(ray& r) const = 0;
		virtual bool intersectB(const ray& r) const = 0;

        virtual point3 sampleSurface(const float& u0, const float& u1) const = 0;
        virtual point3 uniformSampleSurface() const {
            return sampleSurface(sampleUniform(), sampleUniform());
        }

        virtual vec3 getNormal(const point3& p) const = 0;
        virtual float area() const = 0;

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
        virtual void prepGL(GLfloat*& vertexData, GLfloat*& normalData) const = 0;

        /**
         * Performs the required GL draw calls to draw this shape.
         */
        virtual void drawGL() const = 0;
#endif

        virtual long vertexCount() const = 0;

        ray* lastRayTested;

	protected:
		shape* parent;
		aabb boundingBox;
};
