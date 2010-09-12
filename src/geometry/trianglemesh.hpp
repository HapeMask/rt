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

        inline point3 pointLookup(const unsigned int& i) const{
            return pointHeap[i];
        }

        inline vec3 vertNormalLookup(const unsigned int& i) const{
            return vertexNormalHeap[i];
        }

        inline vec2 uvLookup(const unsigned int& i) const{
            return uvHeap[i];
        }

        inline virtual const unsigned long vertexCount() const {
            return totalVertices;
        }

        point3* pointHeap;
        vec3* vertexNormalHeap;
        vec2* uvHeap;

        unsigned long totalVertices;
};

typedef tr1::shared_ptr<triangleMesh> triangleMeshPtr;
