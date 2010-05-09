#include "tracer.hpp"
#include "utility.hpp"

#include "acceleration/intersection.hpp"
#include "mathlib/constants.hpp"
#include "samplers/samplers.hpp"
#include "geometry/triangle.hpp"
#include "materials/bsdf.hpp"

const rgbColor whittedRayTracer::L(const ray& r) const{
    ray r2(r);
	return _L(r2);
}

const rgbColor whittedRayTracer::_L(ray& r, const int& depth) const{
	if(depth > MAXDEPTH){
		return rgbColor(0,0,0);
	}

	const intersection isect = parent->intersect(r);
	if(!isect.hit){
		return 0.f;
	}

    // Handle emissive and specular materials.
    const vec3& normal = isect.p->getNormal(r.origin);
    const bsdf& b = isect.s->getMaterial()->getBsdf();
    const material& mat = *isect.s->getMaterial().get();
    rgbColor Li(0.f);

	if(isect.s->getMaterial()->isEmissive()){
		return clamp(mat.Le());
	}

    // Sample Specular.
    vec3 specDir;
    const rgbColor fr =
        b.sampleF(0, 0, normal, -r.direction, specDir, bxdfType(SPECULAR | REFLECTION));

    if(!fr.isBlack()){
        ray r2(r.origin, specDir);
        Li += fr * _L(r2, depth+1) * abs(dot(specDir, normal));
    }

    const rgbColor ft =
        b.sampleF(0, 0, normal, -r.direction, specDir, bxdfType(SPECULAR | TRANSMISSION));

    if(!ft.isBlack()){
        ray r2(r.origin, specDir);
        Li += ft * _L(r2, depth+1) * abs(dot(specDir, normal));
    }

    // For now, materials are either pure specular or pure diffuse.
    if(!Li.isBlack()){
        return clamp(Li);
    }

    // Diffuse stuff.
	const rgbColor c = mat.sample(r.origin, vec3(0,0,0), -r.direction, bxdfType(DIFFUSE | REFLECTION));;
	for(unsigned int i=0; i<parent->numLights(); ++i){
        const lightPtr li = parent->getLight(i);
		const point3 lightPosition = li->getPosition();
		const float lightDist = (lightPosition - r.origin).length();

		// Test for shadowing early.
		ray shadowRay(point3(r.origin), normalize(lightPosition - r.origin));
		shadowRay.tMax = lightDist;
		const intersection isect2 = parent->intersect(shadowRay);
		if(isect2.hit){
            continue;
		}

		Li += (c * dot(normal, normalize(lightPosition - r.origin)) *
		(1.f / (lightDist*lightDist))) * li->getPower() * li->getColor();
	}

    return clamp(Li);
}
