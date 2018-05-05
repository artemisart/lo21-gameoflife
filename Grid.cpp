#include "Grid.h"

template<typename T>
Grid1D<T>::Grid1D(std::size_t size) :
	size(size),
	values(size, 0)
{}

template<typename T>
Grid2D<T>::Grid2D(std::size_t height, std::size_t width) :
	height(height), width(width),
	values(height * width, 0)
{}

template<typename T>
Grid1D<T>::~Grid1D()
{
	delete values;
}


template<typename T>
Grid2D<T>::~Grid2D()
{
	delete values;
}

template<typename T>
T Grid1D<T>::getCell(const Index1D idx) const
{
    return values[idx.i];
}

template<typename T>
T Grid2D<T>::getCell(const Index2D idx) const
{
    return values[idx.row * this->width + idx.col]; //faire une fonction getIndex() ?
}

template<typename T>
void Grid1D<T>::setCell(const Index1D idx, const T value)
{
	values[idx.i] = value;
}

template<typename T>
void Grid2D<T>::setCell(const Index2D idx, const T value)
{
	values[idx.row * this->width + idx.col] = value;
}
