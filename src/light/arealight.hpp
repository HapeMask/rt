#include "light.hpp"
#include "geometry/triangle.hpp"

class areaLight : public light {
    public:
        areaLight(const point3& p, const float& pow, const rgbColor& c,
                const vec3& vA, const vec3& vB);

        virtual const intersection intersect(const ray& r) const;
        virtual const bool intersectB(const ray& r) const;

        inline virtual const bool isPointSource() const {
            return false;
        }

        inline virtual const float pdf(const point3& p) const {
            return invArea;
        }

        virtual const float pdf(const point3& p, const vec3& wi) const;

        virtual const rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pd) const;

        virtual const vec3 getNormal(const point3& p) const {
            return normal;
        }

        inline virtual const point3 uniformSampleSurface() const {
            point3 samplePoint;
            sampleRectangle(samplePoint, a, b, position, sampleUniform(), sampleUniform());
            return samplePoint;
        }

    private:
		const vec3 a,b, normal;
		const triangle tri1, tri2;
        const float area, invArea;
};
