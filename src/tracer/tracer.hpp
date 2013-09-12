#pragma once

#include <memory>
#include <cfloat>

#include "color/color.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "materials/bsdf.hpp"

#include "acceleration/intersection.hpp"

using std::shared_ptr;

static const rgbColor ERROR_COLOR = {FLT_MAX,0.f,0.f};
static const int MAX_DEPTH = 4;

class scene;
class rayTracer {
    public:
        rayTracer(const scene& p) : parent(p) {}

        virtual rgbColor L(const ray& r) const = 0;
        virtual ~rayTracer() {}

    protected:
        rgbColor sampleOneLight(const point3& p, const vec3& wo, const intersection& isect, const bsdf& bsdf) const;
        rgbColor sampleAllLights(const point3& p, const vec3& wo, const intersection& isect, const bsdf& bsdf) const;
        rgbColor sampleDirect(const point3& p, const vec3& wo, const intersection& isect,
                const bsdf& bsdf, const light& light) const;

        const scene& parent;
};

class whittedRayTracer : public rayTracer {
    public:
        whittedRayTracer(const scene& p) : rayTracer(p) {}

        virtual rgbColor L(const ray& r) const;

    private:
        rgbColor _L(ray& r, const int& depth = 0) const;
};

static const float pathContinueProbability = 0.5f;

class pathTracer : public rayTracer {
    public:
        pathTracer(const scene& p) : rayTracer(p), rrThreshold(3) {}

        pathTracer(const scene& p, const int& rrt) : rayTracer(p), rrThreshold(rrt) {}

        virtual rgbColor L(const ray& r) const;

    private:
        template <const bool recursiveSpecular>
        rgbColor _L(ray& r, const int depth = 0) const;

        int rrThreshold;
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
        virtual rgbColor L(const ray& r) const;

    private:
        void createPath(ray& r, vector<pathPoint>& points) const;
        rgbColor tracePath(const vector<pathPoint>& points) const;
};

typedef shared_ptr<rayTracer> rayTracerPtr;
