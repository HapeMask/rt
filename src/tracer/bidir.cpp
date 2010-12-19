#include "tracer.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "light/light.hpp"
#include "acceleration/intersection.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"
#include "scene/scene.hpp"

const rgbColor bdpt::L(const ray& r) const {
    ray eyeRay(r);

    // Don't bounce off lights if the eye ray hits one, just return the color.
	if(parent.getLight(0)->intersect(r).hit){
		return parent.getLight(0)->L(r);
	}

    // Pick a random light and start a path on it in a random direction.
    int i = sampleRange(sampleUniform(), 0, parent.numLights()-1);

    // Sample the surface to find the start of the light path. 
    const point3 p = parent.getLight(i)->uniformSampleSurface();
    const vec3& normal = parent.getLight(i)->getNormal(p);

    // Pick a random direction to start the light path.
    vec3 wi;
    uniformSampleHemisphere(wi);

    // Make up a coordinate system to convert the sampled direction from
    // light normal space -> world space.
    vec3 binormal, tangent;
    makeCoordinateSystem(normal, binormal, tangent);
    wi = normalize(bsdfToWorld(wi, normal, binormal, tangent));

	// Add the sampled point on the light as the start of the light path.
    rgbColor L(0.f);

    return L;
}

void bdpt::createPath(ray& r, vector<pathPoint>& points) const {
}

const rgbColor bdpt::tracePath(const vector<pathPoint>& points) const{
}
