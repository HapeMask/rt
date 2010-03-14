#include <stdint.h>

#ifndef __COLORRT__
#define __COLORRT__

class color {
	public:
		virtual const float r() const = 0;
		virtual const float g() const = 0;
		virtual const float b() const = 0;

		const uint8_t R() const{
			return 255.f * r();
		}

		const uint8_t G() const{
			return 255.f * g();
		}

		const uint8_t B() const{
			return 255.f * b();
		}
};

class rgbColor : public color {
	public:
		rgbColor(const int& r, const int& g, const int& b){
			r_ = r;
			g_ = g;
			b_ = b;
		}

		rgbColor(const color& c){
			r_ = c.r();
			g_ = c.g();
			b_ = c.b();
		}

		const float r() const{
			return r_;
		}

		const float g() const{
			return g_;
		}

		const float b() const{
			return b_;
		}

		float& r(){
			return r_;
		}

		float& g(){
			return g_;
		}

		float& b(){
			return b_;
		}

		const rgbColor inverse() const;
		void invert();

	private:
		float r_;
		float g_;
		float b_;
};
#endif
