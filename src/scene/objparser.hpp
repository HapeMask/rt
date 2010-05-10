#ifndef __RT_OBJ__
#define __RT_OBJ__

#include "geometry/triangle.hpp"
#include <vector>
using namespace std;

class objParser{
    public:
        static const vector<trianglePtr> parse(const string& filename);
};
#endif

