#ifndef GRID_H
#define GRID_H

#include<vector>
#include "Index.h"

template<typename T, typename I>
class Grid
{
public:
	Grid();
	virtual ~Grid();
	// typedef T EtatType;
	// typedef I IndexType;
	virtual T getCell(const I idx) const = 0;
	virtual void setCell(const I idx, const T value) = 0;
};


template<typename T>
class Grid1D : public Grid<T,Index1D>
{
private:
	std::size_t size;
	std::vector<T> values;

public:
	Grid1D(const std::size_t size);
	virtual ~Grid1D();

	virtual T getCell(const Index1D idx) const;
	virtual void setCell(const Index1D idx, const T value);
};


template<typename T>
class Grid2D : public Grid<T, Index2D>
{
private:
	std::size_t height, width;
	std::vector<T> values;

public:
	Grid2D(const std::size_t height, const std::size_t width);
	virtual ~Grid2D();

	virtual T getCell(const Index2D idx) const;
	virtual void setCell(const Index2D idx, const T value);
};

#endif // GRID_H
