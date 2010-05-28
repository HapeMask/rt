#pragma once
//#warning "TEST"

#include "intersection.hpp"
#include "geometry/primitive.hpp"

#include <typeinfo>

#include <vector>
#include <tr1/memory>

using namespace std;
using tr1::shared_ptr;

enum AXIS{AXIS_X=0,AXIS_Y=1,AXIS_Z=2,AXIS_LEAF=3};

class scene;
class accelerator {
	public:
        virtual ~accelerator() {}

		virtual const intersection intersect(ray& r) const = 0;
		virtual const bool intersectB(const ray& r) const = 0;

		virtual void build(const scene& s) = 0;
};

typedef shared_ptr<accelerator> acceleratorPtr;

inline bool aabbCmpX(primitive* a, primitive* b) { return (a->getBounds().mid().x() < b->getBounds().mid().x()); }
inline bool aabbCmpY(primitive* a, primitive* b) { return (a->getBounds().mid().y() < b->getBounds().mid().y()); }
inline bool aabbCmpZ(primitive* a, primitive* b) { return (a->getBounds().mid().z() < b->getBounds().mid().z()); }

/*
inline int aabbCmpX(const void* a, const void* b) {
    const primitive* pa = *((primitive**)a);
    const primitive* pb = *((primitive**)b);

    if(pa->getBounds().mid().x() > pb->getBounds().mid().x()){
        return 1;
    }else if(pa->getBounds().mid().x() < pb->getBounds().mid().x()){
        return -1;
    }else{
        return 0;
    }
}

inline int aabbCmpY(const void* a, const void* b) {
    const primitive* pa = *((primitive**)a);
    const primitive* pb = *((primitive**)b);

    if(pa->getBounds().mid().y() > pb->getBounds().mid().y()){
        return 1;
    }else if(pa->getBounds().mid().y() < pb->getBounds().mid().y()){
        return -1;
    }else{
        return 0;
    }
}

inline int aabbCmpZ(const void* a, const void* b) {
    const primitive* pa = *((primitive**)a);
    const primitive* pb = *((primitive**)b);

    if(pa->getBounds().mid().z() > pb->getBounds().mid().z()){
        return 1;
    }else if(pa->getBounds().mid().z() < pb->getBounds().mid().z()){
        return -1;
    }else{
        return 0;
    }
}
*/
