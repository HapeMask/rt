#pragma once

#include <stdint.h>
#include <iterator>
#include <iostream>
#include <vector>

template <typename T>
class arraylist{
    template <typename S>
    friend std::ostream& operator<<(std::ostream& out, const arraylist<S>& l);

    public:
        arraylist();
        arraylist(const arraylist<T>& list);
        arraylist(const std::vector<T>& vec);
        arraylist(const int& size);

        ~arraylist(){
            delete[] data;
        }

        void add(const T& x);

        T remove(const int& index);
        T pop(){
            remove(_size-1);
        }

        const uint32_t& size() const {
            return _size;
        }

        arraylist<T>& operator=(const arraylist<T>& list);
        arraylist<T>& operator=(const std::vector<T>& vec);

        T& operator[](const int& index);
        const T& operator[](const int& index) const;

        class iterator : public std::iterator<std::random_access_iterator_tag, T>{
            public:
                iterator() : p(NULL) {}
                iterator(T* x): p(x) {}
                iterator(const iterator& it) : p(it.p) {}

                inline iterator& operator++(){
                    p++;
                    return *this;
                }

                inline iterator& operator++(int) {
                    ++p;
                    return *this;
                }

                inline iterator& operator--(){
                    p--;
                    return *this;
                }

                inline iterator& operator--(int) {
                    --p;
                    return *this;
                }

                inline iterator operator+(const int& x){
                    return iterator(p + x);
                }

                inline iterator& operator+=(const int& x){
                    p += x;
                    return *this;
                }

                inline iterator operator-(const int& x){
                    return iterator(p - x);
                }

                inline iterator& operator-=(const int& x){
                    p -= x;
                    return *this;
                }

                inline ptrdiff_t operator-(const iterator& x){
                    return p - x.p;
                }

                inline bool operator>(const iterator& x){
                    return p > x.p;
                }

                inline bool operator<(const iterator& x){
                    return p < x.p;
                }

                inline bool operator==(const iterator& it) {
                    return it.p == p;
                }

                inline bool operator!=(const iterator& it) {
                    return it.p != p;
                }

                inline T& operator*() {
                    return *p;
                }

            private:
                T* p;
        };

        inline iterator begin() const {
            return iterator(data);
        }

        inline iterator end() const {
            return iterator(data + _size);
        }

    private:
        uint32_t _size, _maxsize;
        void _grow();
        void _shrink();

        T* data;
};

template <typename T>
arraylist<T>::arraylist(){
    _size = 0;
    _maxsize = 8;
    data = new T[_maxsize];
}

template <typename T>
arraylist<T>::arraylist(const int& size){
    _size = size;
    _maxsize = size;
    data = new T[_maxsize];
}

template <typename T>
arraylist<T>::arraylist(const arraylist<T>& list){
    _size = list._size;
    _maxsize = list._maxsize;
    data = new T[_maxsize];

    for(int i=0; i<list.size(); ++i){
        data[i] = list.data[i];
    }
}

template <typename T>
arraylist<T>::arraylist(const std::vector<T>& vec){
    _size = vec.size();
    _maxsize = vec.capacity();
    data = new T[_maxsize];

    for(int i=0; i<vec.size(); ++i){
        data[i] = vec[i];
    }
}

template <typename T>
arraylist<T>& arraylist<T>::operator=(const arraylist<T>& list){
    delete[] data;

    _size = list._size;
    _maxsize = list._maxsize;
    data = new T[_maxsize];

    for(int i=0; i<list.size(); ++i){
        data[i] = list.data[i];
    }

    return *this;
}

template <typename T>
arraylist<T>& arraylist<T>::operator=(const std::vector<T>& vec){
    delete[] data;

    _size = vec.size();
    _maxsize = vec.capacity();
    data = new T[_maxsize];

    for(int i=0; i<vec.size(); ++i){
        data[i] = vec[i];
    }

    return *this;
}

template <typename T>
T& arraylist<T>::operator[](const int& index){
#ifdef DEBUG
    assert(index < _size);
#endif
    return data[index];
}

template <typename T>
const T& arraylist<T>::operator[](const int& index) const{
#ifdef DEBUG
    assert(index < _size);
#endif
    return data[index];
}

template <typename T>
void arraylist<T>::add(const T& x){
    if(_size == _maxsize){
        _grow();
    }

    data[_size] = x;
    ++_size;
}

template <typename T>
T arraylist<T>::remove(const int& index){
#ifdef DEBUG
    assert(index < _size);
#endif

    const T x = data[index];
    for(int i=index; i<_size; ++i){
        data[i] = data[i+1];
    }

    if(_size-- == _maxsize/2){
        _shrink();
    }

    return x;
}

template <typename T>
void arraylist<T>::_grow(){
    const T* oldData = data;
    T* newData = new T[_maxsize * 2];
    for(int i=0; i<_size; ++i){
        newData[i] = oldData[i];
    }

    delete[] oldData;
    data = newData;
    _maxsize *= 2;
}

template <typename T>
void arraylist<T>::_shrink(){
    const T* oldData = data;
    T* newData = new T[_maxsize / 2];
    for(int i=0; i<_size; ++i){
        newData[i] = oldData[i];
    }

    delete[] oldData;
    data = newData;
    _maxsize /= 2;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const arraylist<T>& l){
    typename arraylist<T>::iterator it = l.begin();
    out << '[';

    for(it; it != l.end(); ++it){
        out << *it;
        if(it + 1 != l.end()){
            out << ", ";
        }
    }
    out << ']';
    return out;
}
