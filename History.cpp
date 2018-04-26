#include "History.h"
#include<iostream>


template<typename T>
History<T>::History(const int size): size(size){
    current = 0;
    int i=0;
    while(i<size){
        ring[i]=new T;
        i++;
    }

}

template<typename T>
History<T>::~History(){
    int i=0;
    while(i<size){
        delete ring[i];
        i++;
    }
}

template<typename T>
void History<T>::setStart (const T &start){
    ring[0]=start;
    current=0;
}

template<typename T>
T& History<T>::getLast() const{
    return ring[current];
}

template<typename T>
T& History<T>::get(int i) const{
    if(i>=size){
        std::cout << "Cet état n'a pas été enregistré, seulement les"<< size << " derniers états on été enregristrès \n";
        return 0;
    }else {
        return ring[i];
    }
}
