#include "light.hpp"

class sphereLight : public light {
    public:
        sphereLight(const point3& p, const float& pow, const rgbColor& c, const float& r);

        virtual const intersection intersect(const ray& r) const;
        virtual const bool intersectB(const ray& r) const;

        inline virtual const bool isPointSource() const {
            return false;
        }

        inline virtual const vec3 getNormal(const point3& p) const {
            return normalize(p - location);
        }

        inline virtual const float pdf(const point3& p) const {
            return invArea;
        }

        virtual const float pdf(const point3& p, const vec3& wi) const;

        virtual const rgbColor sampleL(const point3& p, vec3& wi, const float& u0, const float& u1, float& pd) const;

        inline virtual const point3 uniformSampleSurface() const {
            vec3 v;
            uniformSampleSphere(v);
            return location + radius * v;
        }

    private:
        const float radius, radius2;
        const float area, invArea;
};

