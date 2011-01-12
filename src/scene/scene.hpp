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

		const lightPtr& getLight(const int& i) const {
			return lights[i];
		}

        const size_t numEmitters() const {
            return emitters.size();
        }

        shapePtr getEmitter(const int& i) const {
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

        inline const camera& getCamera() const {
            return *cam;
        }

        inline camera& getCamera() {
            return *cam;
        }

		void build();

        void dumpToVbo(GLfloat* vertexVbo, GLfloat* normalVbo) const;
        void drawGL() const;

        inline const rgbColor L(const float& x, const float& y) const {
            return rt->L(cam->getRay(x, y));
        }

        inline const long vertexCount() const {
            return totalVertices;
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

        long totalVertices;
};

typedef shared_ptr<scene> scenePtr;
