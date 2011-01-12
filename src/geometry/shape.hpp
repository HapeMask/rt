#pragma once
#include <memory>
#include <cmath>
#include <vector>

#include "primitive.hpp"
#include "materials/material.hpp"
#include "color/color.hpp"
#include "samplers/samplers.hpp"

using std::shared_ptr;
using std::vector;

class shape {
	public:
		virtual ~shape() {}

		void setMaterial(materialPtr m);
		void addPrimitive(primitivePtr p);

		void setMaterial(material* m);
		void addPrimitive(primitive* p);

		materialPtr getMaterial() const {
			return mat;
		}

		const vector<primitivePtr>& getPrimitives() const {
			return prims;
		}

		const aabb& getBounds(){
			return bounds;
		}

        const point3 sampleSurface(vec3& normal, const float& u0, const float& u1) const {
            const primitive& prim = *prims[sampleRange(sampleUniform(), 0,prims.size()-1)].get();
            const point3& p = prim.sampleSurface(u0, u1);
            normal = prim.getNormal(p);
            return p;
        }

        const float& pdf() const {
            return pdf_;
        }

        const float& area() const {
            return area_;
        }

        virtual const long vertexCount() const {
            long count = 0;

            for(size_t i=0; i<prims.size(); ++i){
                count += prims[i]->vertexCount();
            }

            return count;
        }

#ifdef RT_USE_QT
        /**
         * Prepares any data required to draw this shape on an OpenGL
         * framebuffer. Takes an optional pointer to some data as an argument.
         *
         * Data is usually a vertex array.
         */
        virtual void prepGL(GLfloat*& vertexData, GLfloat*& normalData) const {
            for(size_t i=0; i<prims.size(); ++i){
                prims[i]->prepGL(vertexData, normalData);
            }
        }

        /**
         * Performs the required GL draw calls to draw this shape.
         */
        virtual void drawGL() const {
            for(size_t i=0; i<prims.size(); ++i){
                prims[i]->drawGL();
            }
        }
#endif

	protected:
        float area_, pdf_;
		vector<primitivePtr> prims;
		materialPtr mat;
		aabb bounds;
};

typedef shared_ptr<shape> shapePtr;
