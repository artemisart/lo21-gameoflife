#include "Grid.h"

template<typename T>
Grid1D<T>::Grid1D(std::size_t size) :
	size(size)
{
    int i=0;
    while(i<size){
        values[i]= new T;
        values[i]=0;
        i++;
    }
}

template<typename T>
Grid2D<T>::Grid2D(std::size_t height, std::size_t width) :
	height(height), width(width)
{
    int i=0;
    int j=0;
    while(i<height){
        while(j<width){
            values[i][j]= new T;
            values[i][j]= 0;
            j++;
        }
        i++;
    }

}

template<typename T>
Grid1D<T>::~Grid1D(){
    int i=0;
    while (i<size) {
        delete values[i];
        i++;
    }
}


template<typename T>
Grid2D<T>::~Grid2D(){
    int i=0, j=0;
    while (i<height) {
        while(j<width){
            delete values[i][j];
            i++;
        }
        j++;
    }
}

template<typename T>
T Grid1D<T>::getCell(const Index1D idx) const {
    return values[idx.i];
}

template<typename T>
T Grid2D<T>::getCell(const Index2D idx)const{
    return values[idx.col][idx.row];
}

template<typename T>
void  Grid1D<T>::setCell(const Index1D idx, const T value){
    values[idx.i]=value;
}

template<typename T>
void Grid2D<T>::setCell(const Index2D idx, const T value){
    values[idx.row][idx.col]=value;
}
