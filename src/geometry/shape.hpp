#pragma once

#include "primitive.hpp"
#include "materials/material.hpp"
#include "color/color.hpp"
#include "samplers/samplers.hpp"

#include <cmath>
#include <vector>
#include <tr1/memory>
using namespace std;
using tr1::shared_ptr;

class shape {
	public:
		virtual ~shape() {}

		void setMaterial(materialPtr m);
		void addPrimitive(primitivePtr p);

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

	protected:
        float area_, pdf_;
		vector<primitivePtr> prims;
		materialPtr mat;
		aabb bounds;
};

typedef shared_ptr<shape> shapePtr;
