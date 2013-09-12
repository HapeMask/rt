#include "light.hpp"

class pointLight : public light {
    public:
        pointLight(const point3& p, const float& pow, const rgbColor& c);

        inline virtual bool isPointSource() const {
            return true;
        }

        virtual point3 uniformSampleSurface() const {
            return position;
        }

        virtual vec3 getNormal(const point3& p) const {
            return vec3(0.f, 0.f, 0.f);
        }

        virtual rgbColor sampleL(const point3& p, vec3& wi,
                const float& u0, const float& u1, float& pdf) const;
};
