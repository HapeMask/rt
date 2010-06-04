#pragma once

#include "geometry/meshtriangle.hpp"
#include "geometry/trianglemesh.hpp"
#include "geometry/shape.hpp"

#include "mathlib/vector.hpp"
#include <vector>
using namespace std;

class objParser{
    public:
        static const void parse(const string& filename, triangleMesh* p);
};
