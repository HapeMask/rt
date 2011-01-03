#pragma once

#include <iostream>
using std::ostream;

#include "sse.hpp"

class addOperator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            return addps(lhs.eval(), rhs.eval());
        }
};

class subOperator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            return subps(lhs.eval(), rhs.eval());
        }
};

class mulOperator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            return mulps(lhs.eval(), rhs.eval());
        }
};

class divOperator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            return divps(lhs.eval(), rhs.eval());
        }
};

class negOperator {
    public:
        template <typename rhs_t>
        static inline const __m128 eval(const rhs_t& rhs) {
            return subps(zerops(), rhs.eval());
        }
};

class minOperator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            return minps(lhs.eval(), rhs.eval());
        }
};

class maxOperator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            return maxps(lhs.eval(), rhs.eval());
        }
};

class halfVectorOperator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            const __m128 v = addps(lhs.eval(), rhs.eval());
            const __m128 v1 = mulps(v, v);
            const __m128 v2 = addps(v1, shufps(v1, v1, shuffle(2, 3, 0, 1)));

            return divps(
                    v,
                    sqrtps(
                        addps(
                            v2,
                            shufps(v2, v2, shuffle(0, 1, 2, 3))))
                    );
        }
};

class dotOperator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            const __m128 v1 = mulss(mulps(lhs.eval(), rhs.eval()), zerops());
            const __m128 v2 = addps(v1, shufps(v1, v1, shuffle(2, 3, 0, 1)));

            return addps(v2, shufps(v2, v2, shuffle(0, 1, 2, 3)));
        }
};

class dot4Operator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            const __m128 v1 = mulps(lhs.eval(), rhs.eval());
            const __m128 v2 = addps(v1, shufps(v1, v1, shuffle(2, 3, 0, 1)));

            return addps(v2, shufps(v2, v2, shuffle(0, 1, 2, 3)));
        }
};

class crossOperator {
    public:
        template <typename lhs_t, typename rhs_t>
        static inline const __m128 eval(const lhs_t& lhs, const rhs_t& rhs) {
            const __m128 a = lhs.eval();
            const __m128 b = rhs.eval();
            const __m128 v1 = mulps(
                    shufps(a, a, shuffle(0, 2, 3, 1)),
                    shufps(b, b, shuffle(0, 3, 1, 2))
                );
            const __m128 v2 = mulps(
                    shufps(a, a, shuffle(0, 3, 1, 2)),
                    shufps(b, b, shuffle(0, 2, 3, 1))
                );

            return subps(v1, v2);
        }
};

class sqrtOperator {
    public:
        template <typename expression_t>
        static inline const __m128 eval(const expression_t& expr) {
            return sqrtps(expr.eval());
        }
};

class norm2Operator {
    public:
        template <typename expression_t>
        static inline const __m128 eval(const expression_t& expr) {
            const __m128 v1 = mulps(expr.eval(), expr.eval());
            const __m128 v2 = addps(v1, shufps(v1, v1, shuffle(2, 3, 0, 1)));

            return addps(v2, shufps(v2, v2, shuffle(0, 1, 2, 3)));
        }
};

class normOperator {
    public:
        template <typename expression_t>
        static inline const __m128 eval(const expression_t& expr) {
            const __m128 v1 = mulps(expr.eval(), expr.eval());
            const __m128 v2 = addps(v1, shufps(v1, v1, shuffle(2, 3, 0, 1)));

            return sqrtps(addps(v2, shufps(v2, v2, shuffle(0, 1, 2, 3))));
        }
};

class normalizeOperator {
    public:
        template <typename expression_t>
        static inline const __m128 eval(const expression_t& expr) {
            const __m128 e = expr.eval();
            const __m128 v1 = mulps(e, e);
            const __m128 v2 = addps(v1, shufps(v1, v1, shuffle(2, 3, 0, 1)));

            // The __m128 contains 4 copies of the norm so just divide.
            return divps(
                    e,
                    sqrtps(
                        addps(
                            v2,
                            shufps(v2, v2, shuffle(0, 1, 2, 3))))
                    );
        }
};

template <typename type, typename operation> class expressionVec;
template <typename lhs, typename rhs, typename operation> class expressionVecVec;
template <typename lhs, typename operation> class expressionVecFloat;
template <typename lhs, typename operation> class expressionFloatVec;

template <typename lhsType>
class expression {
    public:
        inline const lhsType& asActualType() const {
            return *static_cast<const lhsType*>(this);
        }

        inline lhsType& asActualType() {
            return *static_cast<lhsType*>(this);
        }

        template <typename rhsType>
        inline const expressionVecVec<lhsType, rhsType, addOperator>
        operator+(const expression<rhsType>& rhs) const;

        template <typename rhsType>
        inline const expressionVecVec<lhsType, rhsType, subOperator>
        operator-(const expression<rhsType>& rhs) const;

        inline const expressionVec<lhsType, negOperator>
        operator-() const;

        template <typename rhsType>
        inline const expressionVecVec<lhsType, rhsType, mulOperator>
        operator*(const expression<rhsType>& rhs) const;

        inline const expressionVecFloat<lhsType, mulOperator>
        operator*(const float& rhs) const;

        template <typename rhsType>
        inline const expressionVecVec<lhsType, rhsType, divOperator>
        operator/(const expression<rhsType>& rhs) const;

        inline const expressionVecFloat<lhsType, divOperator>
        operator/(const float& rhs) const;
};

class expressionFloat : public expression<expressionFloat> {
    public:
        inline expressionFloat(const float& f) : value(f) {}

        inline const __m128 eval() const {
            return _mm_set1_ps(value);
        }

        const float& value;
};

template <typename lhs_t, typename rhs_t, typename operation>
class expressionVecVec : public expression<expressionVecVec<lhs_t, rhs_t, operation> > {
    public:
        expressionVecVec(const lhs_t& l, const rhs_t& r) : lhs(l), rhs(r) {}

        inline const __m128 eval () const {
            return operation::eval(lhs, rhs);
        }

        inline operator float() const {
            float f = 0.f;
            storess(operation::eval(lhs, rhs), &f);
            return f;
        }

        const lhs_t& lhs;
        const rhs_t& rhs;
};

template <typename lhs_t, typename operation>
class expressionVecFloat : public expression<expressionVecFloat<lhs_t, operation> > {
    public:
        expressionVecFloat(const lhs_t& l, const float& r) : lhs(l), rhs(r) {}

        inline const __m128 eval () const {
            return operation::eval(lhs, rhs);
        }

        inline operator float() const {
            float f = 0.f;
            storess(operation::eval(lhs, rhs), &f);
            return f;
        }

        const lhs_t& lhs;
        const expressionFloat& rhs;
};

template <typename rhs_t, typename operation>
class expressionFloatVec : public expression<expressionFloatVec<rhs_t, operation> > {
    public:
        expressionFloatVec(const float& l, const rhs_t& r) : lhs(l), rhs(r) {}

        inline const __m128 eval() const {
            return operation::eval(lhs, rhs);
        }

        inline operator float() const {
            float f = 0.f;
            storess(operation::eval(lhs, rhs), &f);
            return f;
        }

        const expressionFloat& lhs;
        const rhs_t& rhs;
};

template <typename type, typename operation>
class expressionVec : public expression<expressionVec<type, operation> > {
    public:
        expressionVec(const type& v) : vector(v) {}

        inline const __m128 eval() const {
            return operation::eval(vector);
        }

        inline operator float() const {
            float f = 0.f;
            storess(operation::eval(vector), &f);
            return f;
        }

        const type& vector;
};

class vec4 : public expression<vec4> {
    public:
        vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}

        /*
         * Value Constructors
         */
        vec4(const float& a, const float& b, const float& c, const float& d) :
            x(a), y(b), z(c), w(d) {}

        vec4(const float& f) :
            x(f), y(f), z(f), w(f) {}

        vec4(const __m128& vec) : vector(vec) {}

        /*
         * Expression Constructors
         */
        template <typename lhs_t, typename rhs_t, typename operation>
        vec4(const expressionVecVec<lhs_t, rhs_t, operation>& expr) :
            vector(expr.eval()) {}

        template <typename lhs_t, typename operation>
        vec4(const expressionVecFloat<lhs_t, operation>& expr) :
            vector(expr.eval()) {}

        template <typename rhs_t, typename operation>
        vec4(const expressionFloatVec<rhs_t, operation>& expr) :
            vector(expr.eval()) {}

        template <typename type, typename operation>
        vec4(const expressionVec<type, operation>& expr) :
            vector(expr.eval()) {}

        /*
         * Access Operators
         */
		inline const float& operator()(const int& index) const {
            return *((&x) - index);
		}

		inline float& operator()(const int& index) {
            return *((&x) - index);
		}

        inline const __m128 eval() const {
            return vector;
        }

        /*
         * Overloaded Operate/Assign Operators
         */
        template <typename T>
        inline void operator+=(const T& rhs) {
            *this = (*this) + rhs;
        }

        template <typename T>
        inline void operator-=(const T& rhs) {
            *this = (*this) - rhs;
        }

        template <typename T>
        inline void operator*=(const T& rhs) {
            *this = (*this) * rhs;
        }

        template <typename T>
        inline void operator/=(const T& rhs) {
            *this = (*this) / rhs;
        }

        /*
         * Assignment (Used to take an expression tree with some type of root
         * and evaluate it into this vector.
         */
        template <typename expr_t>
        inline void operator=(const expression<expr_t>& expr) {
            // We need the actual type of the expression root in order to call
            // eval since the base class doesn't have any eval().
            vector = expr.asActualType().eval();
        }

        union ALIGN_16 {
            __m128 vector;
           struct {
               float w;
               float z;
               float y;
               float x;
           };
           struct {
               float q;
               float r;
               float t;
               float s;
           };
        };
};

class vec3 : public vec4 {
    public:
        vec3() : vec4() {}

        vec3(const float& a, const float& b, const float& c) : vec4(a, b, c, 0.f) {}
        vec3(const float& f) : vec4(f, f, f, 0.f) {}
        vec3(const __m128& vec) : vec4(vec) {}

        inline const bool operator==(const vec3& v){
            return ((v.x == x) &&
                (v.y == y) &&
                (v.z == z));
        }

        template <typename lhs_t, typename rhs_t, typename operation>
        vec3(const expressionVecVec<lhs_t, rhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename lhs_t, typename operation>
        vec3(const expressionVecFloat<lhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename rhs_t, typename operation>
        vec3(const expressionFloatVec<rhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename type, typename operation>
        vec3(const expressionVec<type, operation>& expr) :
            vec4(expr.eval()) {}
};

class vec2 : public vec4 {
    public:
        vec2() : vec4() {}

        vec2(const float& a, const float& b) : vec4(a, b, 0.f, 0.f) {}
        vec2(const float& f) : vec4(f, f, 0.f, 0.f) {}
        vec2(const __m128& vec) : vec4(vec) {}

        template <typename lhs_t, typename rhs_t, typename operation>
        vec2(const expressionVecVec<lhs_t, rhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename lhs_t, typename operation>
        vec2(const expressionVecFloat<lhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename rhs_t, typename operation>
        vec2(const expressionFloatVec<rhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename type, typename operation>
        vec2(const expressionVec<type, operation>& expr) :
            vec4(expr.eval()) {}
};

class point3 : public vec4 {
    public:
        point3() : vec4() {}

        point3(const float& a, const float& b, const float& c) : vec4(a, b, c, 0.f) {}
        point3(const float& f) : vec4(f, f, f, 0.f) {}
        point3(const __m128& point) : vec4(point) {}

        template <typename lhs_t, typename rhs_t, typename operation>
        point3(const expressionVecVec<lhs_t, rhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename lhs_t, typename operation>
        point3(const expressionVecFloat<lhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename rhs_t, typename operation>
        point3(const expressionFloatVec<rhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename type, typename operation>
        point3(const expressionVec<type, operation>& expr) :
            vec4(expr.eval()) {}
};

class point2 : public vec4 {
    public:
        point2() : vec4() {}

        point2(const float& a, const float& b) : vec4(a, b, 0.f, 0.f) {}
        point2(const float& f) : vec4(f, f, 0.f, 0.f) {}
        point2(const __m128& point) : vec4(point) {}

        template <typename lhs_t, typename rhs_t, typename operation>
        point2(const expressionVecVec<lhs_t, rhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename lhs_t, typename operation>
        point2(const expressionVecFloat<lhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename rhs_t, typename operation>
        point2(const expressionFloatVec<rhs_t, operation>& expr) :
            vec4(expr.eval()) {}

        template <typename type, typename operation>
        point2(const expressionVec<type, operation>& expr) :
            vec4(expr.eval()) {}
};

/*
 * Operator Implementations
 *
 * NOTE: All of these require calling asActualType() on the lhs/rhs of the
 * expressions in order to instantiate the proper templates.
 */
template <typename lhsType>
template <typename rhsType>
inline const expressionVecVec<lhsType, rhsType, addOperator>
expression<lhsType>::operator+(const expression<rhsType>& rhs) const {
    return expressionVecVec<lhsType, rhsType, addOperator>(
            this->asActualType(), rhs.asActualType());
}

template <typename lhsType>
template <typename rhsType>
inline const expressionVecVec<lhsType, rhsType, subOperator>
expression<lhsType>::operator-(const expression<rhsType>& rhs) const {
    return expressionVecVec<lhsType, rhsType, subOperator>(
            this->asActualType(), rhs.asActualType());
}

template <typename lhsType>
inline const expressionVec<lhsType, negOperator>
expression<lhsType>::operator-() const {
    return expressionVec<lhsType, negOperator>(this->asActualType());
}

template <typename lhsType>
template <typename rhsType>
inline const expressionVecVec<lhsType, rhsType, mulOperator>
expression<lhsType>::operator*(const expression<rhsType>& rhs) const {
    return expressionVecVec<lhsType, rhsType, mulOperator>(
            this->asActualType(), rhs.asActualType());
}

template <typename lhsType>
inline const expressionVecFloat<lhsType, mulOperator>
expression<lhsType>::operator*(const float& rhs) const {
    return expressionVecFloat<lhsType, mulOperator>(
            this->asActualType(), rhs);
}

template <typename rhsType>
inline const expressionFloatVec<rhsType, mulOperator>
operator*(const float& lhs, const expression<rhsType>& rhs) {
    return expressionFloatVec<rhsType, mulOperator>(
            lhs, rhs.asActualType());
}

template <typename lhsType>
template <typename rhsType>
inline const expressionVecVec<lhsType, rhsType, divOperator>
expression<lhsType>::operator/(const expression<rhsType>& rhs) const {
    return expressionVecVec<lhsType, rhsType, divOperator>(
            this->asActualType(), rhs.asActualType());
}

template <typename lhsType>
inline const expressionVecFloat<lhsType, divOperator>
expression<lhsType>::operator/(const float& rhs) const {
    return expressionVecFloat<lhsType, divOperator>(
            this->asActualType(), rhs);
}

template <typename rhsType>
inline const expressionFloatVec<rhsType, divOperator>
operator/(const float& lhs, const expression<rhsType>& rhs) {
    return expressionFloatVec<rhsType, divOperator>(
            lhs, rhs.asActualType());
}

template <typename lhsType, typename rhsType>
inline const expressionVecVec<lhsType, rhsType, minOperator>
min(const expression<lhsType>& lhs, const expression<rhsType>& rhs) {
    return expressionVecVec<lhsType, rhsType, minOperator>(
            lhs.asActualType(), rhs.asActualType());
}

template <typename lhsType, typename rhsType>
inline const expressionVecVec<lhsType, rhsType, maxOperator>
max(const expression<lhsType>& lhs, const expression<rhsType>& rhs) {
    return expressionVecVec<lhsType, rhsType, maxOperator>(
            lhs.asActualType(), rhs.asActualType());
}

template <typename lhsType, typename rhsType>
inline const expressionVecVec<lhsType, rhsType, halfVectorOperator>
halfVector(const expression<lhsType>& lhs, const expression<rhsType>& rhs) {
    return expressionVecVec<lhsType, rhsType, halfVectorOperator>(
            lhs.asActualType(), rhs.asActualType());
}

template <typename lhsType, typename rhsType>
inline const expressionVecVec<lhsType, rhsType, dotOperator>
dot(const expression<lhsType>& lhs, const expression<rhsType>& rhs) {
    return expressionVecVec<lhsType, rhsType, dotOperator>(
            lhs.asActualType(), rhs.asActualType());
}

template <typename lhsType, typename rhsType>
inline const expressionVecVec<lhsType, rhsType, dot4Operator>
dot4(const expression<lhsType>& lhs, const expression<rhsType>& rhs) {
    return expressionVecVec<lhsType, rhsType, dot4Operator>(
            lhs.asActualType(), rhs.asActualType());
}

template <typename lhsType, typename rhsType>
inline const expressionVecVec<lhsType, rhsType, crossOperator>
cross(const expression<lhsType>& lhs, const expression<rhsType>& rhs) {
    return expressionVecVec<lhsType, rhsType, crossOperator>(
            lhs.asActualType(), rhs.asActualType());
}

template <typename type>
inline const expressionVec<type, sqrtOperator>
sqrt(const expression<type>& expr) {
    return expressionVec<type, sqrtOperator>(expr.asActualType());
}

template <typename type>
inline const expressionVec<type, normOperator>
norm(const expression<type>& expr) {
    return expressionVec<type, normOperator>(expr.asActualType());
}

template <typename type>
inline const expressionVec<type, norm2Operator>
norm2(const expression<type>& expr) {
    return expressionVec<type, norm2Operator>(expr.asActualType());
}

template <typename type>
inline const expressionVec<type, normalizeOperator>
normalize(const expression<type>& expr) {
    return expressionVec<type, normalizeOperator>(expr.asActualType());
}

ostream& operator<<(ostream& out, const vec2& v);
ostream& operator<<(ostream& out, const vec3& v);
ostream& operator<<(ostream& out, const vec4& v);
ostream& operator<<(ostream& out, const __m128& m);
