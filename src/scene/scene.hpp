#pragma once

#include <vector>
#include <memory>

#ifdef RT_USE_QT
#include <GL/glew.h>
#endif

#include "acceleration/accelerator.hpp"
#include "acceleration/intersection.hpp"

#include "light/light.hpp"
#include "camera/camera.hpp"

#include "mathlib/vector.hpp"

#include "geometry/shape.hpp"
#include "geometry/aabb.hpp"
#include "geometry/primitive.hpp"

#include "tracer/tracer.hpp"

using std::shared_ptr;
using std::vector;

class scene {
	public:
		scene();
		scene(accelerator* a);

		void addLight(light* p);
		void addShape(shape* s);

		/**
		 * NOTE: Destroys the previous accelerator.
		 */
		void setAccelerator(acceleratorPtr a);

		acceleratorPtr getAccelerator() { return accel; }

		intersection intersect(ray& r) const;
		bool intersectB(const ray& r) const;

		const vector<shared_ptr<shape>>& getShapes() const {
			return shapes;
		}

		int numLights() const {
			return lights.size();
		}

		const light& getLight(const int& i) const {
			return *lights[i];
		}

        size_t numEmitters() const {
            return emitters.size();
        }

        const shape& getEmitter(const int& i) const {
            return *emitters[i];
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

        inline const camera& getCamera() const {
            return *cam;
        }

        inline camera& getCamera() {
            return *cam;
        }

		void build();

        void dumpToVbo(GLfloat* vertexVbo, GLfloat* normalVbo) const;
        void drawGL() const;

        inline rgbColor L(const float& x, const float& y) const {
            return rt->L(cam->getRay(x, y));
        }

        inline long vertexCount() const {
            return totalVertices;
        }

	private:
		vector<shared_ptr<shape>> shapes;
		vector<shared_ptr<shape>> emitters;
		vector<unique_ptr<light>> lights;

		cameraPtr cam;
        rayTracerPtr rt;
		acceleratorPtr accel;

		bool needsBuilding;
		aabb bounds;

        long totalVertices;
};

typedef shared_ptr<scene> scenePtr;
