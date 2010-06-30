#pragma once

#include <vector>
#include <tr1/memory>

#include "acceleration/accelerator.hpp"
#include "acceleration/intersection.hpp"

#include "light/light.hpp"
#include "camera/camera.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"

#include "geometry/shape.hpp"
#include "geometry/aabb.hpp"
#include "geometry/primitive.hpp"

#include "tracer/tracer.hpp"

using namespace std;
using tr1::shared_ptr;

class scene {
	public:
		scene();
		scene(accelerator* a);

		void addShape(shapePtr s);
		void addLight(lightPtr p);

		void addShape(shape* s);
		void addLight(light* p);

		/**
		 * NOTE: Destroys the previous accelerator.
		 */
		void setAccelerator(acceleratorPtr a);
		acceleratorPtr getAccelerator() { return accel; }

		const intersection intersect(ray& r) const;
		const bool intersectB(const ray& r) const;

		const vector<shapePtr>& getShapes() const {
			return shapes;
		}

		const size_t numLights() const {
			return lights.size();
		}

		const lightPtr& getLight(const unsigned int& i) const {
			return lights[i];
		}

        const size_t numEmitters() const {
            return emitters.size();
        }

        shapePtr getEmitter(const unsigned int& i) const {
            return emitters[i];
        }

		const aabb& getBounds() const{
			return bounds;
		}

		inline void setCamera(cameraPtr p){
            cam = p;
        }

		inline void setTracer(rayTracerPtr p){
            rt = p;
        }

        const camera& getCamera() const {
            return *cam;
        }

		void build();

        inline const rgbColor L(const float& x, const float& y) const {
            return rt->L(cam->getRay(x, y));
        }

	private:
		vector<shapePtr> shapes;
		vector<shapePtr> emitters;
		vector<lightPtr> lights;

		cameraPtr cam;
        rayTracerPtr rt;
		acceleratorPtr accel;

		bool needsBuilding;
		aabb bounds;
};

typedef shared_ptr<scene> scenePtr;
