#ifndef GRID_H
#define GRID_H

#include <functional>
#include <vector>
#include <fstream>
#include <QFile>
#include <QIODevice>
#include

#include "Index.h"

template <typename T, typename I>
class Grid {
public:
	Grid() {}
	virtual ~Grid() {}
	// typedef T EtatType;
	// typedef I IndexType;
	virtual T getCell(const I idx) const = 0;
	virtual void setCell(const I idx, const T value) = 0;

	virtual void iterate_set(const std::function<T(const I index)> functor) = 0;
	virtual void save(const QFile fich);
};

template <typename T>
class Grid1D : public Grid<T, Index1D> {
private:
	std::size_t size = 0;
	std::vector<T> values;

public:
	Grid1D(const std::size_t size): size(size), values(size, 0)
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

	virtual void iterate_set(const std::function<T(const Index1D)> functor)
	{
		for (Index1D i; i.i < size; ++i.i)
			setCell(i, functor(i));
	}

	virtual bool save(const QFile fich)
	{	
		try{
			fich.open(QIODevice::Truncate);
			fich.putChar(dynamic_cast<char>(1));
			fich.putChar(',');
			fich.putChar(dynamic_cast<char>(this->size));
			fich.putChar(',');
			for(Index1D i; i.i < (this->size) ; ++i.i)
			{
				char nb = dynamic_cast<char>(this.getCell(i)); 
				fich.putChar(nb);
			}
			fich.close();
		}
		catch(exception const& e)
		{
			cerr << "erreur: " << e.what() << endl;
		}
		return true;
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

	virtual void iterate_set(const std::function<T(const Index2D)> functor)
	{
		for (Index2D i; i.row < height; ++i.row)
			for (i.col = 0; i.col < width; ++i.col)
				setCell(i, functor(i));
	}

	virtual void save(const QFile fich)
	{
		try{
			fich.open(QIODevice::Truncate);
			fich.putChar(dynamic_cast<char>(2));
			fich.putChar(',');
			fich.putChar(dynamic_cast<char>(this->height));
			fich.putChar(',');
			fich.putChar(dynamic_cast<char>(this->width));
			fich.putChar(',');
			for(Index2D i; i.row < height; ++i.row)
				for (i.col = 0; i.col < width; ++i.col)
				{
					char nb = dynamic_cast<char>(this.getCell(i)); 
					fich.putChar(nb);
				}
			fich.close();
		}
		catch(exception const& e)
		{
			cerr << "erreur: " << e.what() << endl;
		}
		return true;
	}

};

// TODO fonctions de sauvegarde
// TODO createNew pour allouer une nouvelle grille de même types

#endif // GRID_H
