#ifndef __RT_SCENE__
#define __RT_SCENE__

#include <vector>

#include "geometry/intersectable.hpp"
#include "acceleration/accelerator.hpp"

using namespace std;

class scene : public intersectable{
	public:
		scene();
		scene(accelerator* a);
		~scene();

		void addShape(intersectable* p);

		/**
		 * NOTE: Destroys the previous accelerator.
		 */
		void setAccelerator(accelerator* a);

		virtual bool intersect(ray& r, point3& p);
		void build();

	private:
		vector<intersectable*> shapes;
		accelerator* accel;
		bool needsBuilding;
};
#endif
