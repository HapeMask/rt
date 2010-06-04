#pragma once

#include <iostream>
#include "scanner.hpp"
using namespace std;

class sceneloader{
    public:
        static void load(istream& in, scene& s){
            scanner scan(&in);
            Bison::Parser parser(scan, s);
            int result = parser.parse();
            if(result != 0){
                cerr << "Parse error. Exiting." << endl;
                exit(result);
            }
        }
    private:
        sceneloader();
        sceneloader(const sceneloader& s);
};