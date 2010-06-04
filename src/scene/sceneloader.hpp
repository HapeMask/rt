#pragma once

#include <iostream>
#include "scanner.hpp"
using namespace std;

class sceneloader{
    public:
        static void load(istream& in, scene& s){
            scanner scan(&in);
            Bison::Parser parser(scan, s);
            parser.parse();
        }
    private:
        sceneloader();
        sceneloader(const sceneloader& s);
};
