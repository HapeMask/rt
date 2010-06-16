#include "bidir.hpp"
#include "utility.hpp"

#include "color/color.hpp"
#include "light/light.hpp"
#include "acceleration/intersection.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/point.hpp"

const rgbColor bdpt::L(const ray& r) const {
    ray eyeRay(r);
    vector<pathPoint> eyePath, lightPath;

	if(parent->getLight(0)->intersect(r).hit){
		return parent->getLight(0)->L(r);
	}

    // Add the eye point first.
    const pathPoint p0 = {r.origin, 0.f, 0.f, noIntersect};
    eyePath.push_back(p0);
    createPath(eyeRay, eyePath);

    // Pick a random light and start a path on it in a random direction.
    unsigned int i = sampleRange(sampleUniform(), 0, parent->numLights()-1);
    while(parent->getLight(i)->isPointSource()){
        i = sampleRange(sampleUniform(), 0, parent->numLights()-1);
    }

    // Sample the surface to find the start of the light path. 
    const point3 p = parent->getLight(i)->uniformSampleSurface();
    const vec3& normal = parent->getLight(i)->getNormal();

    vec3 wi;
    uniformSampleHemisphere(wi);

    // Make up a coordinate system to convert the sampled direction from
    // light normal space -> world space.
    vec3 binormal, tangent;
    makeCoordinateSystem(normal, binormal, tangent);
    wi = normalize(bsdfToWorld(wi, normal, binormal, tangent));

	// Add the sampled point on the light as the start of the light path.
	intersection iL(noIntersect);
	iL.li = parent->getLight(i).get();
	const pathPoint lp0 = {p, 0.f, 0.f, iL};
	lightPath.push_back(lp0);

    ray lightRay(p, wi);
    createPath(lightRay, lightPath);

    ray rConnector(eyePath.back().p, normalize(lightPath.back().p - eyePath.back().p));
    rConnector.tMax = (lightPath.back().p - eyePath.back().p).length() - EPSILON;

    // Connect the paths with a visibility ray (if possible).
    if(!parent->intersectB(rConnector)){
        // Merge the paths, then trace it (TODO: use MIS).

		// This loop adds the light points in reverse order because the tracing
		// starts from the light.
        for(int j=0; j<lightPath.size(); ++j){
            eyePath.push_back(lightPath[(lightPath.size()-1)-j]);
        }

        // TODO: Remove this.
        if(eyePath.size() < 2){
            return eyePath[1].isect.li ? eyePath[1].isect.li->getColor() : 0.f;
        }else{
            eyePath[eyePath.size()-1].isect.li = parent->getLight(i).get();
            return tracePath(eyePath);
        }
    }else{
        return eyePath[1].isect.li ? eyePath[1].isect.li->getColor() : 0.f;
    }
}

void bdpt::createPath(ray& r, vector<pathPoint>& points) const {
    for(unsigned int pathLength = 0; ; ++pathLength){
        // Copy the ray since we need the original for the light test below and
        // scene::intersect() modifies it.
        const ray rOrig(r);
        const intersection isect = parent->intersect(r);

        if(!isect.hit){
            break;
        }

        const material& mat = isect.li ? *isect.li->getMaterial().get() : *isect.s->getMaterial().get();
        const bsdf& bsdf = mat.getBsdf();
        const vec3& normal = isect.shadingNormal;

        // Convert the current ray direction to BSDF space.
        const vec3 wo = worldToBsdf(-r.direction, isect);

        vec3 wi;
        float pdf = 0.f;

        // Sample the BSDF to find the next direction.
        bxdfType sampledType;
        const rgbColor f = bsdf.sampleF(sampleUniform(),sampleUniform(),sampleUniform(),wo, wi, ALL, sampledType, pdf);

        pathPoint p = {r.origin, f, pdf, isect, sampledType};
        points.push_back(p);

        if(f.isBlack() || pdf == 0.f){
            break;
        }
        wi = normalize(bsdfToWorld(wi, isect));

        // Potentially terminate the path after 4 bounces.
        if(pathLength > 4){
            const float continueProbability = 0.5f;
            if(sampleUniform() > continueProbability){
                break;
            }
        }

        // Construct the next ray.
        r.direction = wi;
        r.invDir = 1.f/wi;
        r.tMax = MAX_FLOAT;
        r.tMin = EPSILON;
    }
}

const rgbColor bdpt::tracePath(const vector<pathPoint>& points) const{
    // Initial direction is always the ray from the eye.
    rgbColor throughput = 1.f, L = 0.f;
    bool lastBounceWasSpecular = false;

    for(size_t i=1; i<points.size()-2; ++i){
        const intersection& isect = points[i].isect;
        const material& mat = isect.li ? *isect.li->getMaterial().get() : *isect.s->getMaterial().get();
        const bsdf& b = mat.getBsdf();

        // Construct the incident and outgoing directions.
        const vec3 wo = worldToBsdf(normalize(points[i-1].p - points[i].p), isect);
        vec3 wi;
		wi = normalize(points[i+1].p - points[i].p);

        if((i == 0 || lastBounceWasSpecular) && isect.li){
            L += throughput * isect.li->L(ray(points[i].p, wo));
        }

        L += throughput * (sampleAllLights(points[i].p, wo, isect, b) + mat.Le());
        throughput *= points[i].f * fabs(dot(wi, isect.shadingNormal)) / points[i].pdf;
        //throughput /= 0.5f;
        lastBounceWasSpecular = (points[i].sampledType & SPECULAR) != 0;
    }

	const intersection& isect = points.back().isect;
	const material& mat = isect.li ? *isect.li->getMaterial().get() : *isect.s->getMaterial().get();
	const bsdf& b = mat.getBsdf();
	const vec3 wo = worldToBsdf(normalize(points[points.size()-2].p - points.back().p), isect);

	L += throughput * (sampleAllLights(points.back().p, wo, isect, b) + mat.Le());
	return L;
}
