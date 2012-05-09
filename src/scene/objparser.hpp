#pragma once

#include "geometry/meshtriangle.hpp"
#include "geometry/trianglemesh.hpp"
#include "geometry/shape.hpp"

#include "mathlib/vector.hpp"

class objParser{
    public:
        static void parse(const string& filename, triangleMesh* p, const bool smooth = true);
};
