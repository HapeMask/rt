#pragma once

#include "matrix.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"

class transform3d{
	public:
		transform3d(){}

		transform3d(const mat4& m, const mat4& i){
			mat = m;
			inv = i;
		}

		transform3d(const mat4& m){
			mat = m;
			inv = m.inverse();
		}

		const mat4& matrix() const{
			return mat;
		}

		const mat4& inverse() const{
			return inv;
		}

		const point3 apply(const point3& p) const;
		const vec3 apply(const vec3& v) const;
		const ray apply(const ray& r) const;

		const point3 unapply(const point3& p) const;
		const vec3 unapply(const vec3& v) const;
		const ray unapply(const ray& r) const;

		const transform3d operator*(const transform3d& t) const;
		transform3d& operator*=(const transform3d& t);

	private:
		mat4 mat, inv;
};

const transform3d translate(const float& dx, const float& dy, const float& dz);
const transform3d scale(const float& sx, const float& sy, const float& sz);
const transform3d lookAt(const point3& pos, const point3& look, const vec3& up);
const transform3d perspective(const float& fov, const float& near, const float& far);
