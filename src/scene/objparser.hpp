#pragma once

#include "geometry/triangle.hpp"
#include "mathlib/vector.hpp"
#include <vector>
using namespace std;

class objParser{
    public:
        static const vector<trianglePtr> parse(const string& filename, const vec3& offset, const float& scale);
};
