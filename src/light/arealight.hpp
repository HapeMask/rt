#include "light.hpp"
#include "geometry/triangle.hpp"

class areaLight : public light {
    public:
        areaLight(const point3& p, const float& pow, const rgbColor& c,
                const vec3& vA, const vec3& vB);

        virtual intersection intersect(const ray& r) const;
        virtual bool intersectB(const ray& r) const;

        inline virtual bool isPointSource() const {
            return false;
        }

        inline virtual float pdf(const point3& p) const {
            return invArea;
        }

        virtual float pdf(const point3& p, const vec3& wi) const;

        virtual rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pd) const;

        virtual vec3 getNormal(const point3& p) const {
            return normal;
        }

        inline virtual point3 uniformSampleSurface() const {
            point3 samplePoint;
            sampleRectangle(samplePoint, a, b, position, sampleUniform(), sampleUniform());
            return samplePoint;
        }

    private:
        vec3 a,b, normal;
        triangle tri1, tri2;
        float area, invArea;
};
