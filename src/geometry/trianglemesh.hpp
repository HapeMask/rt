#pragma once

#include <memory>
#include <cmath>
#include <vector>

#include "shape.hpp"
#include "triangle.hpp"
#include "primitive.hpp"

#include "materials/material.hpp"
#include "color/color.hpp"
#include "samplers/samplers.hpp"

using std::vector;

class triangleMesh : public shape {
    public:
        triangleMesh() : pointStorage(NULL), vertexNormalStorage(NULL), uvStorage(NULL), totalVertices(0)
        {}

        virtual ~triangleMesh(){
            if(pointStorage) delete[] pointStorage;
            if(vertexNormalStorage) delete[] vertexNormalStorage;
            if(uvStorage) delete[] uvStorage;
        }

        inline const point3& pointLookup(const int& i) const{
            return pointStorage[i];
        }

        inline const vec3& vertNormalLookup(const int& i) const{
            return vertexNormalStorage[i];
        }

        inline const vec2& uvLookup(const int& i) const{
            return uvStorage[i];
        }

        inline virtual long vertexCount() const {
            return totalVertices;
        }

        point3* pointStorage;
        vec3* vertexNormalStorage;
        vec2* uvStorage;

        long totalVertices;
};

typedef std::shared_ptr<triangleMesh> triangleMeshPtr;
