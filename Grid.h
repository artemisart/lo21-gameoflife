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
	bool wrapAround = false;

	Grid() {}
	virtual ~Grid() {}
	virtual T getCell(const I idx) const = 0;
	virtual void setCell(const I idx, const T value) = 0;
	virtual I getSize() const = 0;

	virtual Grid<T, I>* createNew() const = 0;
	virtual void iterate_get(const std::function<void(const T cell)> functor) const = 0;
	virtual void iterate_set(const std::function<T(const I index)> functor) = 0;
	virtual void save(const std::string& filePath) const = 0;
	virtual void load(const std::string& filePath) =0;
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
		if (this->wrapAround)
			return values[idx.i % values.size()];
		else if (idx.i < 0 || idx.i >= static_cast<int>(values.size()))
			return T();
		else
			return values[idx.i];
	}
	virtual void setCell(const Index1D idx, const T value)
	{
		values.at(idx.i) = value;
	}

	Grid1D<T>* createNew() const
	{
		auto* newGrid = new Grid1D(values.size());
		return newGrid;
	}

	virtual void iterate_get(const std::function<void(const T)> functor) const
	{
		for (Index1D i; i.i < static_cast<int>(values.size()); ++i.i)
			functor(getCell(i));
	}
	virtual void iterate_set(const std::function<T(const Index1D)> functor)
	{
		for (Index1D i; i.i < static_cast<int>(values.size()); ++i.i)
			setCell(i, functor(i));
	}
	virtual void save(const std::string& filePath) const
	{
		try {
			std::cout << "save 1D, "<<filePath << "\n";
			std::ofstream file(filePath, std::ios::out | std::ios::trunc);
			file << "1 " << values.size() << " ";
			iterate_get([&](const T cell) { file << cell << " "; });
			file.close();
		} catch (const std::string& e) {
			std::cout << "erreur: " << e << "\n";
		}
	}

	virtual Index1D getSize() const { return values.size(); }

	virtual void load(const std::string& filePath)
	{
	  try {
		  std::ifstream file(filePath, std::ios::in);
		  T cell;
		  unsigned int it;
		  file >> it;
		  if(it!=1){throw std::string("wrong loading, expected a 1D grid \n");}
		  file >> it;
		  values.resize(it,0);
		  for (Index1D i; i.i < it; ++i.i)
		   {
		      file >> cell;
		      setCell(i, cell);
		   }
		  file.close();
	  } catch (const std::string& e) {
		  std::cout << "erreur: " << e << "\n";
	  }
	}
};

template <typename T>
class Grid2D : public Grid<T, Index2D> {
private:
	int height = 0, width = 0;
	std::vector<T> values;

public:
	Grid2D(const int height, const int width)
		: height(height)
		, width(width)
		, values(height * width, 0)
	{
		if (height < 0 || width < 0)
			throw std::out_of_range("height and width must be >= 0");
	}
	virtual ~Grid2D() {}

	virtual T getCell(const Index2D idx) const
	{
		if (this->wrapAround)
			return values[(idx.row % this->height) * this->width + idx.col % this->width];
		else if (idx.row < 0 || idx.row >= this->height || idx.col < 0 || idx.col >= this->width)
			return T();
		else
			return values[idx.row * this->width + idx.col];
	}
	virtual void setCell(const Index2D idx, const T value)
	{
		values.at(idx.row * this->width + idx.col) = value;
	}
	virtual Index2D getSize() const { return Index2D(height, width); }

	virtual Grid2D<T>* createNew() const
	{
		auto* newGrid = new Grid2D(height, width);
		return newGrid;
	}

	virtual void iterate_get(const std::function<void(const T)> functor) const
	{
		for (Index2D i; i.row < height; ++i.row)
			for (i.col = 0; i.col < width; ++i.col)
				functor(getCell(i));
	}
	virtual void iterate_set(const std::function<T(const Index2D)> functor)
	{
		for (Index2D i; i.row < height; ++i.row)
			for (i.col = 0; i.col < width; ++i.col)
				setCell(i, functor(i));
	}
	virtual void save(const std::string& filePath) const
	{
		try {
			std::ofstream file(filePath, std::ios::out | std::ios::trunc);
			file << "2 " << height << " " << width << " ";
			iterate_get([&](const T cell) { file << cell<< " "; });
			file.close();
		} catch (const std::string& e) {
			std::cout << "erreur: " << e << "\n";
		}
	}
	virtual void load(const std::string& filePath)
	{
	  try {
		  std::ifstream file(filePath, std::ios::in);
		  unsigned int it;
		  file >> it;
		  if(it!=2){throw std::string("wrong loading, expected 2D grid \n");}
		  file >> height; file>> width;
		  T cell;
		  values.resize(height*width,0);
		  for (Index2D i; i.row < height; ++i.row)
		    {
			  for (i.col = 0; i.col < width; ++i.col)
			    {
				  file >> cell;
				  setCell(i, cell);
			    }
		    }
		  file.close();
	  } catch (const std::string& e) {
		  std::cout << "erreur: " << e << "\n";
	  }
	}
};

// TODO fonctions de sauvegarde

#endif // GRID_H
