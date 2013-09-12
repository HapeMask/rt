#pragma once

#include <iostream>
using std::istream;

#include "utility.hpp"
#include "scanner.hpp"

class sceneloader{
    public:
        static void load(istream& in, scene& s){
            scanner scan(&in);
            Bison::Parser parser(scan, s);
            int result = parser.parse();
            if(result != 0){ rt_throw("Parse error."); }
        }
    private:
        sceneloader();
        sceneloader(const sceneloader& s);
};
