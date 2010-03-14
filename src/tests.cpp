#include <iostream>
#include <cassert>

#include "vector.hpp"
//#include "point.hpp"
#include "matrix.hpp"
using namespace std;

int main(int argc, char** argv){
	vec3 u(1, 0, 0);
	vec3 v(0, 1, 0);
	vec2 w(1, 0);
	vec2 x(0, 1);

	/**
	 * vec3 dot() Tests.
	 */
	u = vec3(1, 0, 0);
	v = vec3(0, 1, 0);
	assert(dot(u,v) == 0);

	u = vec3(1,2,3);
	v = vec3(1,2,3);
	assert(dot(u,v) == 14);

	/**
	 * vec2 dot() Tests.
	 */
	w = vec2(1, 0);
	x = vec2(0, 1);
	assert(dot(w,x) == 0);

	w = vec2(1,2);
	x = vec2(1,2);
	assert(dot(w,x) == 5);
	cout << "dot() passed." << endl;

	/**
	 * Cross Product Tests.
	 */
	u = vec3(1, 0, 0);
	v = vec3(0, 1, 0);
	assert(cross(u,v) == vec3(0,0,1));
	cout << "cross() passed." << endl;

	/**
	 * Point Tests.
	 */
	/*
	point3 p(1,1,1);
	assert((p + vec3(0,1,0)) == point3(1,2,1));
	assert((p - point3(1,0,1)) == vec3(0,1,0));
	cout << "point ops passed" << endl;
	*/
	return 0;
}
