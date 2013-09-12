#include "utility.hpp"
#include "objparser.hpp"

#include "geometry/meshtriangle.hpp"
#include "geometry/trianglemesh.hpp"
#include "geometry/shape.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/vector.hpp"

#include <fstream>
#include <vector>
#include <algorithm>

using std::find;
using std::ifstream;
using std::vector;
using std::cerr;
using std::endl;

typedef struct tps{
    int a;
    int b;
    int c;
} trianglePointSet;

void objParser::parse(const string& filename, triangleMesh* p, const bool smooth){
    ifstream file(filename.c_str());
    vector<meshTriangle*> tris(0);

    if(!file.is_open() || !file.good()){
        rt_throw("Unable to open OBJ file for reading: " + filename);
    }

    string chunk("");

    // Consume any header information.
    while(!file.eof() && chunk != "v"){
        file >> chunk;
    }

    if(file.eof()){
        file.close();
        rt_throw("End of file reached before finding any valid data.");
    }

    vector<point3> points;
    while(!file.eof() && chunk == "v"){
        float x, y, z;

        file >> x;
        file >> y;
        file >> z;
        file >> chunk;

        points.push_back(point3(x, y, z));
    }

    // Fill in the scene's point heap.
    p->pointHeap = new point3[points.size()];
    for(size_t i=0; i<points.size(); ++i){
        p->pointHeap[i] = points[i];
    }

    if(file.eof()){
        rt_throw("End of file reached before making any faces.");
    }

    // Consume any intermediate info.
    while(!file.eof() && chunk != "f" && chunk != "vt"){
        file >> chunk;
    }

    vector<vec2> uvs;
    // Read in texture coords.
    while(!file.eof() && chunk == "vt"){
        float u,v;

        file >> u;
        file >> v;
        file >> chunk;

        uvs.push_back(vec2(u, v));
    }

    if(uvs.size() > 0){
        p->uvHeap = new vec2[uvs.size()];
        for(size_t i=0; i<uvs.size(); ++i){
            p->uvHeap[i] = uvs[i];
        }
    }

    // Consume any intermediate info.
    while(!file.eof() && chunk != "f"){
        file >> chunk;
    }

    vector<vector<meshTriangle*>> vertPolys(points.size());

    // Read in the faces and construct the polys.
    while(!file.eof() && chunk == "f"){
        int vert1, vert2, vert3;
        int uv1, uv2, uv3;

        file >> chunk;
        if(uvs.size() > 0){
            vert1 = strtol(chunk.substr(0, chunk.find('/')).c_str(), NULL, 10);
            uv1 = strtol(chunk.substr(chunk.find('/')+1, chunk.length()).c_str(), NULL, 10);
        }else{
            vert1 = strtol(chunk.c_str(), NULL, 10);
            uv1 = 0;
        }

        file >> chunk;
        if(uvs.size() > 0){
            vert2 = strtol(chunk.substr(0, chunk.find('/')).c_str(), NULL, 10);
            uv2 = strtol(chunk.substr(chunk.find('/')+1, chunk.length()).c_str(), NULL, 10);
        }else{
            vert2 = strtol(chunk.c_str(), NULL, 10);
            uv2 = 0;
        }

        file >> chunk;
        if(uvs.size() > 0){
            vert3 = strtol(chunk.substr(0, chunk.find('/')).c_str(), NULL, 10);
            uv3 = strtol(chunk.substr(chunk.find('/')+1, chunk.length()).c_str(), NULL, 10);
        }else{
            vert3 = strtol(chunk.c_str(), NULL, 10);
            uv3 = 0;
        }

        meshTriangle* tri = new meshTriangle(vert1-1, vert2-1, vert3-1, p);
        if(uvs.size() > 0){
            tri->setUVs(uv1-1, uv2-1, uv3-1);
        }

        tris.push_back(tri);

        vertPolys[vert1-1].push_back(tri);
        vertPolys[vert2-1].push_back(tri);
        vertPolys[vert3-1].push_back(tri);

        file >> chunk;
    }

    p->totalVertices = 3 * tris.size();

    if(smooth){
        // Sum the normals for all faces adjacent to each vert,
        // then store the vertex normal.
        vector<vec3> vertNormals;
        for(size_t i=0; i<points.size(); ++i){
            const vector<meshTriangle*>& vps = vertPolys[i];
            vec3 normal(0.f);

            vector<vec3> addedNorms;
            for(auto vp : vps){
                // TODO: THIS SUCKS...
                if(find(addedNorms.begin(), addedNorms.end(), vp->normal()) == addedNorms.end()){
                    addedNorms.push_back(vp->normal());
                    normal += addedNorms[addedNorms.size() - 1];
                }
            }
            vertNormals.push_back(normalize(normal));
        }

        // Fill in the scene's vert normal heap.
        p->vertexNormalHeap = new vec3[vertNormals.size()];
        for(size_t i=0;i<vertNormals.size(); ++i){
            p->vertexNormalHeap[i] = vertNormals[i];
        }

        for(size_t i=0; i<tris.size(); ++i){
            tris[i]->setVertNormals(tris[i]->aIndex(),
                    tris[i]->bIndex(),
                    tris[i]->cIndex());
        }
    }

    for(size_t i=0; i<tris.size(); ++i){
        p->addPrimitive(tris[i]);
    }

    file.close();

    cerr << "Loaded " << tris.size() << " polygons from " << filename << endl;
}
