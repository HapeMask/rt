#ifndef __RT_SCENE__
#define __RT_SCENE__

#include <vector>

#include "geometry/intersectable.hpp"
#include "geometry/shape.hpp"

#include "acceleration/accelerator.hpp"
#include "acceleration/intersection.hpp"

using namespace std;

class scene {
	public:
		scene();
		scene(accelerator* a);
		~scene();

		void addPrimitive(primitive* p);
		void addShape(const shape& s);
		void addLight(const light* l);

		/**
		 * NOTE: Destroys the previous accelerator.
		 */
		void setAccelerator(accelerator* a);
		accelerator* getAccelerator() { return accel; }

		const intersection intersect(ray& r) const;
		const intersection intersect1(ray& r) const;
		const bool intersectB(ray& r) const;
		void build();

	private:
		vector<primitive*> shapes;
		vector<light*> lights;

		accelerator* accel;
		bool needsBuilding;
};
#endif
