#pragma once
#include <memory>
#include <cmath>

#include "trianglemesh.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/ray.hpp"
#include "primitive.hpp"
#include "acceleration/intersection.hpp"

class meshTriangle : public primitive {
    public:
        meshTriangle(const int& _a, const int& _b, const int& _c, triangleMesh* p);

        virtual intersection intersect(ray& r) const;
        virtual bool intersectB(const ray& r) const;

        const int& aIndex() const{ return points[0]; }
        const int& bIndex() const{ return points[1]; }
        const int& cIndex() const{ return points[2]; }

        virtual vec3 getNormal(const point3& p) const { return normal_; }
        inline const vec3& normal() const { return normal_; }

        inline virtual const point3& a() const{
            return parentMesh()->pointStorage[points[0]];
        }

        inline virtual const point3& b() const{
            return parentMesh()->pointStorage[points[1]];
        }

        inline virtual const point3& c() const{
            return parentMesh()->pointStorage[points[2]];
        }

        inline const vec3& vertNormal(const int& i) const {
            return parentMesh()->vertNormalLookup(vertNormals[i]);
        }

        inline const vec2& uv(const int& i) const {
            return parentMesh()->uvLookup(uvs[i]);
        }

        inline triangleMesh* parentMesh() {
            return static_cast<triangleMesh*>(parent);
        }

        inline const triangleMesh* parentMesh() const {
            return static_cast<const triangleMesh*>(parent);
        }

        virtual point3 sampleSurface(const float& u0, const float& u1) const;
        inline virtual float area() const { return area_; }

        void setVertNormals(const int& an, const int& bn, const int& cn);
        void setUVs(const int& auv, const int& buv, const int& cuv);

        inline virtual long vertexCount() const { return 3; }

#ifdef RT_USE_QT
        virtual void prepGL(GLfloat*& vertexData, GLfloat*& normalData) const;
        virtual void drawGL() const {};
#endif

    private:
        int points[3];
        int vertNormals[3];
        int uvs[3];

        vec3 normal_, binormal_;
        vec3 B, C;

        float area_;
        bool hasVertNormals, hasUVs;
};

inline point3 meshTriangle::sampleSurface(const float& u0, const float& u1) const {
    point3 ret;
    const triangle t(a(), b(), c());
    sampleTriangle(ret, t, u0, u1);
    return ret;
}

typedef std::shared_ptr<meshTriangle> meshTrianglePtr;
