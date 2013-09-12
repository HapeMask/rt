#pragma once

#include <memory>
#include <string>

using std::string;
using std::shared_ptr;

#ifdef RT_USE_OPENMP
#include <omp.h>
#endif

#include "color/color.hpp"
#include "mathlib/vector.hpp"

static const int NUM_TEXTURE_SLOTS = 6;

enum textureSlot {
    COEFF1 = 0,
    COEFF2 = 1,
    COEFF3 = 2,
    COEFF4 = 3,
    DIFFUSE_COLOR = 4,
    SPECULAR_COLOR = 5
};

class texture2D {
    public:
#ifdef RT_USE_OPENMP
        static bool lookupInitialized;
        static rgbColor** lookupCache;
#endif

        texture2D(const int& w, const int& h) : width(w), height(h) {
#ifdef RT_USE_OPENMP
            initializeLookupCache();
#endif
        }

        texture2D(const string& filename) : width(0), height(0) {
#ifdef RT_USE_OPENMP
            initializeLookupCache();
#endif
        }

        virtual ~texture2D() {}

        rgbColor lookup(const vec2& uv) const;

        int width, height;

    protected:
        virtual const rgbColor& _lookup(const int& x, const int& y) const = 0;

#ifdef RT_USE_OPENMP
    private:
        static void initializeLookupCache() {
#pragma omp critical
            {
            if(!lookupInitialized){
                lookupInitialized = true;
                lookupCache = new rgbColor*[omp_get_max_threads()];
                for(int i=0; i < omp_get_max_threads(); ++i){
                    lookupCache[i] = new rgbColor[NUM_TEXTURE_SLOTS];
                }
            }
            }
        }
#endif
};

class colorTexture2D : public texture2D {
    public:
        colorTexture2D(const int& w, const int& );
        colorTexture2D(const string& filename);
        virtual ~colorTexture2D() {
            if(texels){
                for(int i=0; i < height; ++i){
                    delete[] texels[i];
                }
            }
        }

    protected:
        virtual const rgbColor& _lookup(const int& x, const int& y) const;

    private:
        rgbColor** texels;
};

typedef shared_ptr<texture2D> texture2DPtr;
