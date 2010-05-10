#include "objparser.hpp"
#include "geometry/triangle.hpp"

#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

typedef struct tps{
    int a;
    int b;
    int c;
} trianglePointSet;

const vector<trianglePtr> objParser::parse(const string& filename){
    ifstream file(filename.c_str());
    vector<trianglePtr> tris(0);

    if(!file.is_open() || !file.good()){
        cerr << "Unable to open SMF file for reading: " << filename.c_str() << endl;
        return tris;
    }

    string chunk("");

    // Consume any header information.
    while(!file.eof() && chunk != "v"){
        file >> chunk;
    }

    if(file.eof()){
        cerr << "End of file reached before finding any valid data." << endl;
        file.close();
        return tris;
    }

    vector<point3> points;
    while(!file.eof() && chunk == "v"){
        file >> chunk;
        float x = (float)strtod(chunk.c_str(), NULL);

        file >> chunk;
        if(file.eof()){
            cerr << "End of file reached before making any faces." << endl;
            return tris;
        }
        float y = (float)strtod(chunk.c_str(), NULL);

        file >> chunk;
        if(file.eof()){
            cerr << "End of file reached before making any faces." << endl;
            return tris;
        }
        float z = (float)strtod(chunk.c_str(), NULL);

        file >> chunk;
        if(file.eof()){
            cerr << "End of file reached before making any faces." << endl;
            return tris;
        }

        points.push_back(point3(x, y, z));
    }

    if(file.eof()){
        cerr << "End of file reached before making any faces." << endl;
        return tris;
    }

    // Consume any intermediate info.
    while(!file.eof() && chunk != "f"){
        file >> chunk;
    }

    vector<vector<trianglePtr> > vertPolys(points.size());
    vector<trianglePointSet> triPoints;
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
            return tris;
        }
        const unsigned int vert2 = strtol(chunk.c_str(), NULL, 10);

        file >> chunk;
        if(file.eof()){
            cerr << "End of face reached before making a triangle." << endl;
            return tris;
        }
        const unsigned int vert3 = strtol(chunk.c_str(), NULL, 10);

        trianglePtr tri(new triangle(points[vert1-1], points[vert2-1], points[vert3-1]));
        tris.push_back(tri);

        const trianglePointSet tps = {vert1-1, vert2-1, vert3-1};
        triPoints.push_back(tps);

        vertPolys[vert1-1].push_back(tri);
        vertPolys[vert2-1].push_back(tri);
        vertPolys[vert3-1].push_back(tri);

        file >> chunk;
    }

    // Sum the normals for all faces adjacent to each vert,
    // then store the vertex normal.
    cerr << "TEST" << endl;
    vector<vec3> vertNormals;
    for(size_t i=0; i<points.size(); ++i){
        const vector<trianglePtr>& vps = vertPolys[i];
        vec3 normal(0.f,0.f,0.f);

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

    for(size_t i=0; i<tris.size(); ++i){
        const trianglePointSet& tp = triPoints[i];
        tris[i]->setVertNormals(vertNormals[tp.a], vertNormals[tp.b], vertNormals[tp.c]);
    }

    file.close();

    cerr << "Loaded " << tris.size() << " polygons from " << filename << endl;
    return tris;
}
