#pragma once

#include <omp.h>

#include "color/color.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"
#include "mathlib/ray.hpp"

#include "acceleration/intersection.hpp"

#include <tr1/memory>

using namespace std;
using tr1::shared_ptr;

const int MAX_DEPTH = 4;

class scene;
class rayTracer {
	public:
		rayTracer(const scene& p) : parent(p) {}

		virtual const rgbColor L(const ray& r) const = 0;
        virtual ~rayTracer() {}

	protected:
        const rgbColor sampleOneLight(const point3& p, const vec3& wo, const intersection& isect, const bsdf& bsdf) const;
        const rgbColor sampleAllLights(const point3& p, const vec3& wo, const intersection& isect, const bsdf& bsdf) const;
        const rgbColor sampleDirect(const point3& p, const vec3& wo, const intersection& isect,
                const bsdf& bsdf, const light& light) const;

		const scene& parent;
};

class whittedRayTracer : public rayTracer {
	public:
		whittedRayTracer(const scene& p) : rayTracer(p) {}

		virtual const rgbColor L(const ray& r) const;

	private:
		const rgbColor _L(ray& r, const int& depth = 0) const;
};

static const float pathContinueProbability = 0.5f;

class pathTracer : public rayTracer {
	public:
		pathTracer(const scene& p) : rayTracer(p) {}
		virtual const rgbColor L(const ray& r) const;

	private:
		template <const bool recursiveSpecular>
		const rgbColor _L(ray& r, const int depth = 0) const;
};

struct pathPoint{
    point3 p;
    rgbColor f;
    float pdf;
    intersection isect;
    bxdfType sampledType;
};

class bdpt : public rayTracer {
	public:
		bdpt(const scene& p) : rayTracer(p) {}
		virtual const rgbColor L(const ray& r) const;

	private:
		void createPath(ray& r, vector<pathPoint>& points) const;
        const rgbColor tracePath(const vector<pathPoint>& points) const;
};

typedef shared_ptr<rayTracer> rayTracerPtr;
