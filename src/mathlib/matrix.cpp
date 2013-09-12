#include "matrix.hpp"
#include "vector.hpp"
#include <iostream>

using std::cerr;
using std::endl;

mat4 mat4::operator*(const float& x) const {
    return mat4(*this) *= x;
}

vec4 mat4::operator*(const vec4& u) const{
    float x=0.f,y=0.f,z=0.f,w=0.f;

    for(int i=0; i<4; ++i) {
        x += u(i) * data[0][i];
        y += u(i) * data[1][i];
        z += u(i) * data[2][i];
        w += u(i) * data[3][i];
    }

    return vec4(x,y,z,w);
}

mat4& mat4::operator*=(const float& x) {
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            data[i][j] *= x;
        }
    }

    return (*this);
}

mat4& mat4::operator*=(const mat4& m) {
    (*this) = (*this) * m;
    return (*this);
}

mat4 mat4::operator*(const mat4& m) const{
    float D[4][4];
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            D[i][j] = 0;
            for(int k=0; k<4; ++k) {
                D[i][j] += data[i][k] * m.data[k][j];
            }
        }
    }

    return mat4(D);
}

bool mat4::operator==(const mat4& m) const{
    int test = 1;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            test *= (data[i][j] == m(i,j));
        }
    }
    return test;
}

float mat4::det() const{
    return
        data[0][0]*data[1][1]*data[2][2]*data[3][3] + data[0][0]*data[1][2]*data[2][3]*data[3][1] + data[0][0]*data[1][3]*data[2][1]*data[3][2] +
        data[0][1]*data[1][0]*data[2][3]*data[3][2] + data[0][1]*data[1][2]*data[2][0]*data[3][3] + data[0][1]*data[1][3]*data[2][2]*data[3][0] +
        data[0][2]*data[1][0]*data[2][1]*data[3][3] + data[0][2]*data[1][1]*data[2][3]*data[3][0] + data[0][2]*data[1][3]*data[2][0]*data[3][1] +
        data[0][3]*data[1][0]*data[2][2]*data[3][1] + data[0][3]*data[1][1]*data[2][0]*data[3][2] + data[0][3]*data[1][2]*data[2][1]*data[3][0] -
        data[0][0]*data[1][1]*data[2][3]*data[3][2] - data[0][0]*data[1][2]*data[2][1]*data[3][3] - data[0][0]*data[1][3]*data[2][2]*data[3][1] -
        data[0][1]*data[1][0]*data[2][2]*data[3][3] - data[0][1]*data[1][2]*data[2][3]*data[3][0] - data[0][1]*data[1][3]*data[2][0]*data[3][2] -
        data[0][2]*data[1][0]*data[2][3]*data[3][1] - data[0][2]*data[1][1]*data[2][0]*data[3][3] - data[0][2]*data[1][3]*data[2][1]*data[3][0] -
        data[0][3]*data[1][0]*data[2][1]*data[3][2] - data[0][3]*data[1][1]*data[2][2]*data[3][0] - data[0][3]*data[1][2]*data[2][0]*data[3][1];
}

mat4 mat4::inverse() const{
    mat4 inv;

    inv(0,0)=   data[1][1]*data[2][2]*data[3][3] - data[1][1]*data[2][3]*data[3][2] - data[2][1]*data[1][2]*data[3][3]
        + data[2][1]*data[1][3]*data[3][2] + data[3][1]*data[1][2]*data[2][3] - data[3][1]*data[1][3]*data[2][2];
    inv(1,0) =  -data[1][0]*data[2][2]*data[3][3] + data[1][0]*data[2][3]*data[3][2] + data[2][0]*data[1][2]*data[3][3]
        - data[2][0]*data[1][3]*data[3][2] - data[3][0]*data[1][2]*data[2][3] + data[3][0]*data[1][3]*data[2][2];
    inv(2,0) =   data[1][0]*data[2][1]*data[3][3] - data[1][0]*data[2][3]*data[3][1] - data[2][0]*data[1][1]*data[3][3]
        + data[2][0]*data[1][3]*data[3][1] + data[3][0]*data[1][1]*data[2][3] - data[3][0]*data[1][3]*data[2][1];
    inv(3,0) = -data[1][0]*data[2][1]*data[3][2] + data[1][0]*data[2][2]*data[3][1] + data[2][0]*data[1][1]*data[3][2]
        - data[2][0]*data[1][2]*data[3][1] - data[3][0]*data[1][1]*data[2][2] + data[3][0]*data[1][2]*data[2][1];
    inv(0,1) =  -data[0][1]*data[2][2]*data[3][3] + data[0][1]*data[2][3]*data[3][2] + data[2][1]*data[0][2]*data[3][3]
        - data[2][1]*data[0][3]*data[3][2] - data[3][1]*data[0][2]*data[2][3] + data[3][1]*data[0][3]*data[2][2];
    inv(1,1) =   data[0][0]*data[2][2]*data[3][3] - data[0][0]*data[2][3]*data[3][2] - data[2][0]*data[0][2]*data[3][3]
        + data[2][0]*data[0][3]*data[3][2] + data[3][0]*data[0][2]*data[2][3] - data[3][0]*data[0][3]*data[2][2];
    inv(2,1) =  -data[0][0]*data[2][1]*data[3][3] + data[0][0]*data[2][3]*data[3][1] + data[2][0]*data[0][1]*data[3][3]
        - data[2][0]*data[0][3]*data[3][1] - data[3][0]*data[0][1]*data[2][3] + data[3][0]*data[0][3]*data[2][1];
    inv(3,1) =  data[0][0]*data[2][1]*data[3][2] - data[0][0]*data[2][2]*data[3][1] - data[2][0]*data[0][1]*data[3][2]
        + data[2][0]*data[0][2]*data[3][1] + data[3][0]*data[0][1]*data[2][2] - data[3][0]*data[0][2]*data[2][1];
    inv(0,2) =   data[0][1]*data[1][2]*data[3][3] - data[0][1]*data[1][3]*data[3][2] - data[1][1]*data[0][2]*data[3][3]
        + data[1][1]*data[0][3]*data[3][2] + data[3][1]*data[0][2]*data[1][3] - data[3][1]*data[0][3]*data[1][2];
    inv(1,2) =  -data[0][0]*data[1][2]*data[3][3] + data[0][0]*data[1][3]*data[3][2] + data[1][0]*data[0][2]*data[3][3]
        - data[1][0]*data[0][3]*data[3][2] - data[3][0]*data[0][2]*data[1][3] + data[3][0]*data[0][3]*data[1][2];
    inv(2,2) =  data[0][0]*data[1][1]*data[3][3] - data[0][0]*data[1][3]*data[3][1] - data[1][0]*data[0][1]*data[3][3]
        + data[1][0]*data[0][3]*data[3][1] + data[3][0]*data[0][1]*data[1][3] - data[3][0]*data[0][3]*data[1][1];
    inv(3,2) = -data[0][0]*data[1][1]*data[3][2] + data[0][0]*data[1][2]*data[3][1] + data[1][0]*data[0][1]*data[3][2]
        - data[1][0]*data[0][2]*data[3][1] - data[3][0]*data[0][1]*data[1][2] + data[3][0]*data[0][2]*data[1][1];
    inv(0,3) =  -data[0][1]*data[1][2]*data[2][3] + data[0][1]*data[1][3]*data[2][2] + data[1][1]*data[0][2]*data[2][3]
        - data[1][1]*data[0][3]*data[2][2] - data[2][1]*data[0][2]*data[1][3] + data[2][1]*data[0][3]*data[1][2];
    inv(1,3) =   data[0][0]*data[1][2]*data[2][3] - data[0][0]*data[1][3]*data[2][2] - data[1][0]*data[0][2]*data[2][3]
        + data[1][0]*data[0][3]*data[2][2] + data[2][0]*data[0][2]*data[1][3] - data[2][0]*data[0][3]*data[1][2];
    inv(2,3) = -data[0][0]*data[1][1]*data[2][3] + data[0][0]*data[1][3]*data[2][1] + data[1][0]*data[0][1]*data[2][3]
        - data[1][0]*data[0][3]*data[2][1] - data[2][0]*data[0][1]*data[1][3] + data[2][0]*data[0][3]*data[1][1];
    inv(3,3) =  data[0][0]*data[1][1]*data[2][2] - data[0][0]*data[1][2]*data[2][1] - data[1][0]*data[0][1]*data[2][2]
        + data[1][0]*data[0][2]*data[2][1] + data[2][0]*data[0][1]*data[1][2] - data[2][0]*data[0][2]*data[1][1];

    inv *= 1.f / det();
    return inv;
}

mat4 mat4::transpose() const{
    float D[4][4];
    for(int i=0; i<4; ++i) {
        for(int j=0; j<4; ++j) {
            D[i][j] = data[j][i];
        }
    }
    return mat4(D);
}

ostream& operator<<(ostream& out, const mat4& m){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            out << m(i,j);
            if(j < 3)
                out << ", ";
        }
        if(i < 3)
            out << endl;
    }

    return out;
}
