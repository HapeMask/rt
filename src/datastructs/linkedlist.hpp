#pragma once

#include <stdint.h>
#include <iterator>
#include <iostream>

template <typename T>
class listnode {
    public:
        listnode(const T& x) : data(x), next(NULL), prev(NULL)
        {}

        ~listnode(){
            if(next) delete next;
        }

        T data;
        listnode<T>* next;
        listnode<T>* prev;
};

template <typename T>
class linkedlist{
    template <typename S>
    friend std::ostream& operator<<(std::ostream& out, const linkedlist<S>& l);

    public:
        linkedlist() : _size(0), head(NULL), tail(NULL)
        {}

        ~linkedlist(){
            delete head;
        }

        void push(const T& x);
        void append(const T& x);

        T pop();
        T& top();
        const T& top() const;

        inline const uint32_t& size(){
            return _size;
        }

        inline const bool empty() const {
            return _size == 0;
        }

        class iterator : public std::iterator<std::bidirectional_iterator_tag, T>{
            public:
                iterator(listnode<T>* x): p(x) {}
                iterator(const iterator& it) : p(it.p) {}

                inline iterator& operator++(){
                    p = p->next;
                    return *this;
                }

                inline iterator& operator++(int) {
                    p = p->next();
                    return *this;
                }

                inline iterator& operator--(){
                    p = p->prev;
                    return *this;
                }

                inline iterator& operator--(int) {
                    p = p->prev();
                    return *this;
                }

                inline iterator next(){
                    return p ? iterator(p->next) : NULL;
                }

                inline iterator prev(){
                    return p ? iterator(p->prev) : NULL;
                }

                inline bool operator==(const iterator& it) {
                    return it.p == p;
                }

                inline bool operator!=(const iterator& it) {
                    return it.p != p;
                }

                inline T& operator*() {
                    return p->data;
                }

            private:
                listnode<T>* p;
        };

        inline iterator begin() const {
            return iterator(head);
        }

        inline iterator end() const {
            return iterator(NULL);
        }

    private:
        uint32_t _size;
        listnode<T>* head, *tail;
};

template <typename T>
void linkedlist<T>::push(const T& x){
    listnode<T>* prevHead = head;

    head = new listnode<T>(x);
    head->next = prevHead;
    if(prevHead){
        prevHead->prev = head;
    }

    if(!tail){
        tail = head;
    }
    ++_size;
}

template <typename T>
void linkedlist<T>::append(const T& x){
    listnode<T>* prevTail = tail;

    tail = new listnode<T>(x);
    tail->prev = prevTail;
    if(prevTail){
        prevTail->next = tail;
    }

    if(!head){
        head = tail;
    }
    ++_size;
}

template <typename T>
T linkedlist<T>::pop(){
    const T x = head->data;
    listnode<T>* prevHead = head;

    head = prevHead->next;
    if(head){
        head->prev = NULL;
    }

    if(prevHead){
        prevHead->next = NULL;
    }

    delete prevHead;
    --_size;

    if(_size == 0){
        head = NULL;
        tail = NULL;
    }

    return x;
}

template <typename T>
T& linkedlist<T>::top(){
    return head->data;
}

template <typename T>
const T& linkedlist<T>::top() const{
    return head->data;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const linkedlist<T>& l){
    typename linkedlist<T>::iterator it = l.begin();
    out << '[';

    for(it; it != l.end(); ++it){
        out << *it;
        if(it.next() != NULL){
            out << ", ";
        }
    }
    out << ']';
    return out;
}
