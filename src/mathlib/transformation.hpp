#pragma once

#include "matrix.hpp"
#include "vector.hpp"
#include "point.hpp"
#include "ray.hpp"

class transform3d{
    public:
        transform3d() = default;
        transform3d(const mat4& m, const mat4& i) : mat(m), inv(i) {}
        transform3d(const mat4& m) : mat(m), inv(m.inverse()) {}

        inline const mat4& matrix() const{ return mat; }
        inline const mat4& inverse() const{ return inv; }

        inline point3 apply(const point3& p) const {
            const vec4 v = mat * vec4(p, 1.f);
            return point3(v.x/v.w, v.y/v.w, v.z/v.w);
        }

        inline vec3 apply(const vec3& v) const {
            return vec3(
                    mat(0,0) * v.x + mat(0,1) * v.y + mat(0,2) * v.z,
                    mat(1,0) * v.x + mat(1,1) * v.y + mat(1,2) * v.z,
                    mat(2,0) * v.x + mat(2,1) * v.y + mat(2,2) * v.z
                    );
        }

        inline ray apply(const ray& r) const {
            return ray(apply(r.origin), apply(r.direction));
        }

        inline point3 unapply(const point3& p) const {
            const vec4 v = inv * vec4(p.x, p.y, p.z, 1.f);
            return point3(v.x/v.w, v.y/v.w, v.z/v.w);
        }

        inline vec3 unapply(const vec3& v) const {
            return vec3(
                    inv(0,0) * v.x + inv(0,1) * v.y + inv(0,2) * v.z,
                    inv(1,0) * v.x + inv(1,1) * v.y + inv(1,2) * v.z,
                    inv(2,0) * v.x + inv(2,1) * v.y + inv(2,2) * v.z
                    );
        }

        inline ray unapply(const ray& r) const {
            return ray(unapply(r.origin), unapply(r.direction));
        }

        inline transform3d operator*(const transform3d& t) const {
            return transform3d(*this) *= t;
        }

        inline transform3d& operator*=(const transform3d& t) {
            mat *= t.mat;
            inv = mat.inverse();
            return (*this);
        }

    private:
        mat4 mat, inv;
};

transform3d translate(const float& dx, const float& dy, const float& dz);
transform3d scale(const float& sx, const float& sy, const float& sz);
transform3d lookAt(const point3& pos, const point3& look, const vec3& up);
transform3d perspective(const float& fov, const float& near, const float& far);
