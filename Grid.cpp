#include "Grid.h"

template<typename T>
Grid1D<T>::Grid1D(std::size_t size) :
	size(size)
{}

template<typename T>
Grid2D<T>::Grid2D(std::size_t height, std::size_t width) :
	height(height), width(width)
{}
