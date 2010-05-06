#ifndef __RT_OBJ__
#define __RT_OBJ__

#include "geometry/primitive.hpp"
#include <vector>
using namespace std;

class objParser{
    public:
        static const vector<primitivePtr> parse(const string& filename);
};
#endif

