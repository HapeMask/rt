#include "light.hpp"

class pointLight : public light {
	public:
		pointLight(const point3& p, const float& pow, const rgbColor& c);

        inline virtual const bool isPointSource() const {
            return true;
        }

        virtual const point3 uniformSampleSurface() const {
            return position;
        }

        virtual const rgbColor sampleL(const point3& p, vec3& wi,
                const float& u0, const float& u1, float& pdf) const;
};
