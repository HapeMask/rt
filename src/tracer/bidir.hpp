#pragma once

#include "color/color.hpp"
#include "scene/scene.hpp"

#include "mathlib/ray.hpp"
#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"

#include "tracer.hpp"

struct pathPoint{
    point3 p;
    rgbColor L, throughput;
    float pdf;
};

class bdpt : public rayTracer {
	public:
		bdpt(scene* p) : rayTracer(p) {}
		virtual const rgbColor L(const ray& r) const;

	private:
		void createPath(ray& r, vector<pathPoint>& points) const;
};
