#ifndef GRID_H
#define GRID_H

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Index.h"

template <typename T, typename I>
class Grid {
public:
	Grid() {}
	virtual ~Grid() {}
	virtual T getCell(const I idx) const = 0;
	virtual void setCell(const I idx, const T value) = 0;

	virtual Grid<T, I>* createNew() const = 0;
	virtual void iterate_set(const std::function<T(const I index)> functor) = 0;
	virtual void save(const std::string& nom) const = 0;
};

template <typename T>
class Grid1D : public Grid<T, Index1D> {
private:
	std::vector<T> values;

public:
	Grid1D(const std::size_t size)
		: values(size, 0)
	{
	}
	virtual ~Grid1D() {}

	virtual T getCell(const Index1D idx) const
	{
		return values[idx.i];
	}
	virtual void setCell(const Index1D idx, const T value)
	{
		values[idx.i] = value;
	}

	Grid1D<T>* createNew() const
	{
		auto* newGrid = new Grid1D(values.size());
		newGrid->iterate_set([&](const Index1D i) { return this->getCell(i); });
		return newGrid;
	}

	virtual void iterate_set(const std::function<T(const Index1D)> functor)
	{
		for (Index1D i; i.i < values.size(); ++i.i)
			setCell(i, functor(i));
	}
	virtual void save(const std::string& nom) const
	{
		try {
			std::ofstream fich(nom, std::ios::out | std::ios::trunc);
			fich << "1," << values.size() << ",";
			for (Index1D i; i.i < values.size(); ++i.i) {
				fich << getCell(i);
				fich << ',';
			}
			fich.close();
		} catch (const std::string& e) {
			std::cout << "erreur: " << e << "\n";
		}
	}
};

template <typename T>
class Grid2D : public Grid<T, Index2D> {
private:
	std::size_t height = 0, width = 0;
	std::vector<T> values;

public:
	Grid2D(const std::size_t height, const std::size_t width)
		: height(height)
		, width(width)
		, values(height * width, 0)
	{
	}
	virtual ~Grid2D() {}

	virtual T getCell(const Index2D idx) const
	{
		return values[idx.row * this->width + idx.col];
	}
	virtual void setCell(const Index2D idx, const T value)
	{
		values[idx.row * this->width + idx.col] = value;
	}

	Grid2D<T>* createNew() const
	{
		auto* newGrid = new Grid2D(height, width);
		newGrid->iterate_set([&](const Index2D i) { return this->getCell(i); });
		return newGrid;
	}

	virtual void iterate_set(const std::function<T(const Index2D)> functor)
	{
		for (Index2D i; i.row < height; ++i.row)
			for (i.col = 0; i.col < width; ++i.col)
				setCell(i, functor(i));
	}
	virtual void save(const std::string& nom) const
	{
		try {
			std::ofstream fich(nom, std::ios::out | std::ios::trunc);
			fich << "2," << height << "," << width << ",";
			for (Index2D i; i.row < height; ++i.row)
				for (i.col = 0; i.col < width; ++i.col) {
					fich << getCell(i);
					fich << ',';
				}
			fich.close();
		} catch (const std::string& e) {
			std::cout << "erreur: " << e << "\n";
		}
	}
};

// TODO fonctions de sauvegarde

#endif // GRID_H
