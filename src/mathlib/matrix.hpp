#pragma once

#include "vector.hpp"

class mat4 {
    public:
        /**
         * Default constructor = identity.
         */
        mat4(){
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    data[i][j] = (i==j) ? 1.f : 0.f;
                }
            }
        }

        mat4(const mat4& m){
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    data[i][j] = m(i,j);
                }
            }
        }

        /**
         * Column-vector constructor.
         */
        mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d){
            for(int i=0; i<4; i++){
                data[i][0] = a(i);
                data[i][1] = b(i);
                data[i][2] = c(i);
                data[i][3] = d(i);
            }
        }

        mat4(const float v[4][4]){
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    data[i][j] = v[i][j];
                }
            }
        }

        const float& operator()(const short& i, const short& j) const {
#ifdef DEBUG
            assert(i < 4 && j < 4);
#endif
            return data[i][j];
        }

        float& operator()(const short& i, const short& j){
#ifdef DEBUG
            assert(i < 4 && j < 4);
#endif
            return data[i][j];
        }

        mat4 inverse() const;
        mat4 transpose() const;
        float det() const;

        mat4 operator*(const float& x) const;
        mat4& operator*=(const float& x);

        vec4 operator*(const vec4& u) const;
        mat4& operator*=(const mat4& m);
        mat4 operator*(const mat4& m) const;

        bool operator==(const mat4& m) const;

    private:
        float data[4][4];
};

ostream& operator<<(ostream& out, const mat4& m);
