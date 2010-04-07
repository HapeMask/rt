#include "aabb.hpp"

ostream& operator<<(ostream& out, const aabb& b){
	out << "AABB:\n\tTop: " << b.top <<
		"\n\tBottom: " << b.bottom <<
		"\n\tLeft: " << b.left <<
		"\n\tRight: " << b.right <<
		"\n\tFront: " << b.front <<
		"\n\tBack: " << b.back;
	return out;
}
