#ifndef __RT_SCENE__
#define __RT_SCENE__

#include <vector>
#include <tr1/memory>

#include "acceleration/accelerator.hpp"
#include "acceleration/intersection.hpp"

#include "camera/camera.hpp"

#include "geometry/shape.hpp"
#include "geometry/aabb.hpp"
#include "geometry/primitive.hpp"
#include "light/light.hpp"

using namespace std;
using tr1::shared_ptr;

class scene {
	public:
		scene();
		scene(accelerator* a);

		void addShape(shapePtr s);
		void addEmitter(shapePtr p);
		void addLight(lightPtr p);

		/**
		 * NOTE: Destroys the previous accelerator.
		 */
		void setAccelerator(accelerator* a);
		acceleratorPtr getAccelerator() { return accel; }

		const intersection intersect(ray& r) const;
		const intersection intersectE(ray& r) const;
		const bool intersectEB(const ray& r) const;
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

		const vector<shapePtr>& getEmitters() const {
			return emitters;
		}

		const aabb& getBounds() const{
			return bounds;
		}

		const cameraPtr& getCamera() const{
			return cam;
		}

		void setCamera(cameraPtr p);

		void build();

	private:
		vector<shapePtr> shapes;
		vector<shapePtr> emitters;
		vector<lightPtr> lights;

		cameraPtr cam;
		acceleratorPtr accel;
		bool needsBuilding;
		aabb bounds;
};

typedef shared_ptr<scene> scenePtr;
#endif
