#include "color.hpp"
#include <iostream>
using std::ostream;

ostream& operator<<(ostream& out, const color& c){
	out << "color(" << c.red() << ", " << c.green() << ", " << c.blue() << ")";
	return out;
}
