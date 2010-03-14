#ifndef __RT_SCENE__
#define __RT_SCENE__

#include <vector>

#include "geometry/intersectable.hpp"
#include "acceleration/accelerator.hpp"

using namespace std;

class scene {
	public:
		scene();
		scene(accelerator* a);
		~scene();

		void addShape(intersectable* p);

	private:
		vector<intersectable*> shapes;
		accelerator* accel;
		bool needsBuilding;
};
#endif
