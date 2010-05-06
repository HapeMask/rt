#include "objparser.hpp"
#include "geometry/triangle.hpp"
#include "geometry/primitive.hpp"

#include <fstream>
#include <vector>
using namespace std;

const vector<primitivePtr> objParser::parse(const string& filename){
    ifstream file(filename.c_str());
    vector<primitivePtr> tris(0);

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

        // TODO:
        // Add flag for vertex ordering. NOTE that it uses vert1,vert3,vert2 instead of 1,2,3
        // because my SMF files assume clockwise ordering while the RT assumes CCW ordering.
        tris.push_back(primitivePtr(new triangle(points[vert1-1], points[vert2-1], points[vert3-1])));
        file >> chunk;
    }

    file.close();

    cerr << "Loaded " << tris.size() << " polygons from " << filename << endl;
    return tris;
}
