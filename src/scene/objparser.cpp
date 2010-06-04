#include "objparser.hpp"

#include "geometry/meshtriangle.hpp"
#include "geometry/trianglemesh.hpp"
#include "geometry/shape.hpp"

#include "mathlib/vector.hpp"
#include "mathlib/vector.hpp"

#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const void objParser::parse(const string& filename, triangleMesh* p){
    ifstream file(filename.c_str());
    vector<meshTrianglePtr> tris(0);

    if(!file.is_open() || !file.good()){
        cerr << "Unable to open SMF file for reading: " << filename.c_str() << endl;
    }

    string chunk("");

    // Consume any header information.
    while(!file.eof() && chunk != "v"){
        file >> chunk;
    }

    if(file.eof()){
        cerr << "End of file reached before finding any valid data." << endl;
        file.close();
    }

    vector<point3> points;
    while(!file.eof() && chunk == "v"){
        file >> chunk;
        float x = (float)strtod(chunk.c_str(), NULL);

        file >> chunk;
        if(file.eof()){
            cerr << "End of file reached before making any faces." << endl;
        }
        float y = (float)strtod(chunk.c_str(), NULL);

        file >> chunk;
        if(file.eof()){
            cerr << "End of file reached before making any faces." << endl;
        }
        float z = (float)strtod(chunk.c_str(), NULL);

        file >> chunk;
        if(file.eof()){
            cerr << "End of file reached before making any faces." << endl;
        }

        points.push_back(point3(x, y, z));
    }

    // Fill in the scene's point heap.
    p->pointHeap = new point3[points.size()];
    for(size_t i=0;i<points.size(); ++i){
        p->pointHeap[i] = points[i];
    }

    if(file.eof()){
        cerr << "End of file reached before making any faces." << endl;
    }

    // Consume any intermediate info.
    while(!file.eof() && chunk != "f"){
        file >> chunk;
    }

    vector<vector<meshTrianglePtr> > vertPolys(points.size());
    // Read in the faces and construct the polys.
    while(!file.eof() && chunk == "f"){
        file >> chunk;
        if(file.eof()){
            break;
        }

        const unsigned int vert1 = strtol(chunk.c_str(), NULL, 10);

        file >> chunk;
        if(file.eof()){
            cerr << "End of face reached before making a triangle." << endl;
        }
        const unsigned int vert2 = strtol(chunk.c_str(), NULL, 10);

        file >> chunk;
        if(file.eof()){
            cerr << "End of face reached before making a triangle." << endl;
        }
        const unsigned int vert3 = strtol(chunk.c_str(), NULL, 10);

        meshTrianglePtr tri(new meshTriangle(vert1-1, vert2-1, vert3-1, p));
        tris.push_back(tri);

        vertPolys[vert1-1].push_back(tri);
        vertPolys[vert2-1].push_back(tri);
        vertPolys[vert3-1].push_back(tri);

        file >> chunk;
    }

    // Sum the normals for all faces adjacent to each vert,
    // then store the vertex normal.
    vector<vec3> vertNormals;
    for(size_t i=0; i<points.size(); ++i){
        const vector<meshTrianglePtr>& vps = vertPolys[i];
        vec3 normal(0.f);

        vector<vec3> addedNorms;
        for(size_t j=0; j<vps.size(); ++j){
            // TODO: THIS SUCKS...
            if(find(addedNorms.begin(), addedNorms.end(), vps[j]->normal()) == addedNorms.end()){
                addedNorms.push_back(vps[j]->normal());
                normal += addedNorms[addedNorms.size() - 1];
            }
        }
        //addedNorms.clear();

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
        p->addPrimitive(tris[i]);
    }

    file.close();

    cerr << "Loaded " << tris.size() << " polygons from " << filename << endl;
}
