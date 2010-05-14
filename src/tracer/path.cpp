#include "path.hpp"
#include "color/color.hpp"
#include "light/light.hpp"
#include "acceleration/intersection.hpp"

const rgbColor pathTracer::L(const ray& r) const {
    ray r2(r);
    return _L(r2);
}

const rgbColor pathTracer::_L(ray& r, const int& depth) const {
    rgbColor throughput = 1.f, L = 0.f;
    bool lastBounceWasSpecular = false;

    for(unsigned int pathLength = 0; ; ++pathLength){
        const intersection isect = parent->intersect(r);
        if(!isect.hit){
            return 0;
        }

        const material& mat = *isect.s->getMaterial().get();
        if(pathLength == 0 || lastBounceWasSpecular){
            L += mat.Le() * throughput;
        }

        const bsdf& bsdf = mat.getBsdf();
        const vec3& wo = -r.direction;
        const vec3& normal = isect.shadingNormal;
        L += throughput * sampleOneLight(r.origin, wo, normal, bsdf);
    }

    return L;
}

const rgbColor pathTracer::sampleOneLight(const point3& p, const vec3& wo, const vec3& normal,
        const bsdf& bsdf) const{
    return sampleDirect(p, wo, normal, bsdf, *parent->getLight(0).get());
}

const rgbColor pathTracer::sampleDirect(const point3& p, const vec3& wo, const vec3& normal,
        const bsdf& bsdf, const light& li) const {
    vec3 wi;
    float bsdfPdf;
    /*
    rgbColor L = li.sampleL(p, wi, sampleUniform(), sampleUniform(), lightPdf);

    if(!L.isBlack()){
        const rgbColor f = bsdf.f(wo, wi);
        if(!li.isPointSource()){
            bsdfPdf = bsdf.pdf(wo, wi);
            L += f *
        }else{
        }
    }
    */
}
