#ifndef __RT_TRACER__
#define __RT_TRACER__

#include <omp.h>

#include "color/color.hpp"
#include "scene/scene.hpp"

const unsigned int MAXDEPTH = 20;
const unsigned int areaSamples = 16;

//static unsigned int sampleIndex[8];
class rayTracer {
	public:
		rayTracer(scene* p) : parent(p), imgWidth(p->getCamera()->width()),
        imgHeight(p->getCamera()->height()){
            /*
            samples = new float[imgWidth*imgHeight*areaSamples*2];
            getLDSamples2D(samples, imgWidth*imgHeight*areaSamples);

            for(int i=0;i<8;++i){
                sampleIndex[i] = (imgWidth*imgHeight*areaSamples*2*i) / 8;
            }
            */
        }

		virtual const rgbColor L(const ray& r) const = 0;
        virtual ~rayTracer() {
            //delete[] samples;
        }

	protected:
        /*
        inline void getNextSample(float sample[2]) const {
#ifdef RT_MULTITHREADED
            const unsigned int tid = omp_get_thread_num();
#else
            const unsigned int tid = 0;
#endif
            sample[0] = samples[sampleIndex[tid]];
            sample[1] = samples[sampleIndex[tid]+1];
            sampleIndex[tid] += 2;
        }

        float* samples;
        */
		scene* parent;
        unsigned int imgWidth, imgHeight;
};

class whittedRayTracer : public rayTracer {
	public:
		whittedRayTracer(scene* p) : rayTracer(p) {}

		virtual const rgbColor L(const ray& r) const;

	private:
		const rgbColor _L(ray& r, const int& depth = 0) const;
};
#endif
