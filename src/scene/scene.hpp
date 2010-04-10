#ifndef __RT_SCENE__
#define __RT_SCENE__

#include <vector>
#include <tr1/memory>

#include "acceleration/accelerator.hpp"
#include "acceleration/intersection.hpp"

#include "geometry/shape.hpp"
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

		const vector<lightPtr>& getLights() const {
			return lights;
		}

		const vector<shapePtr>& getEmitters() const {
			return emitters;
		}

		void build();

	private:
		vector<shapePtr> shapes;
		vector<shapePtr> emitters;
		vector<lightPtr> lights;

		acceleratorPtr accel;
		bool needsBuilding;
};

typedef shared_ptr<scene> scenePtr;
#endif
