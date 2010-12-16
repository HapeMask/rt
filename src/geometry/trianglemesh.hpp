#pragma once

#include "shape.hpp"
#include "triangle.hpp"
#include "primitive.hpp"

#include "materials/material.hpp"
#include "color/color.hpp"
#include "samplers/samplers.hpp"

#include <cmath>
#include <vector>
#include <tr1/memory>
using namespace std;

class triangleMesh : public shape {
	public:
		triangleMesh() : pointHeap(NULL), vertexNormalHeap(NULL), uvHeap(NULL), totalVertices(0)
        {}

        ~triangleMesh(){
            if(pointHeap) delete[] pointHeap;
            if(vertexNormalHeap) delete[] vertexNormalHeap;
            if(uvHeap) delete[] uvHeap;
        }

        inline point3 pointLookup(const int& i) const{
            return pointHeap[i];
        }

        inline vec3 vertNormalLookup(const int& i) const{
            return vertexNormalHeap[i];
        }

        inline vec2 uvLookup(const int& i) const{
            return uvHeap[i];
        }

        inline virtual const long vertexCount() const {
            return totalVertices;
        }

        point3* pointHeap;
        vec3* vertexNormalHeap;
        vec2* uvHeap;

        long totalVertices;
};

typedef tr1::shared_ptr<triangleMesh> triangleMeshPtr;
