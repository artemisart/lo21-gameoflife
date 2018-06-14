#ifndef GRID_H
#define GRID_H

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Index.h"

/**
 * @file Grid.h
 * @brief Grid for 1 or 2 dimentional Automaton
 */

/**
 * @brief The Grid abstract class is used to create a Grid which can be subclassed to have 1 dimension or more.
 * A function can also be applied on each cell.
 * A Grid can store any type (int, bool ....)
 */

template <typename T, typename I>
class Grid {
public:
    bool wrapAround = false;

    Grid() {}
    Grid(const Grid& grid) = delete;
    void operator=(const Grid& grid) = delete;

    virtual ~Grid() {}

    /**
     * @brief Access a cell thanks to its index
     * @param idx : an index structure that stores the place of the cell
     * @return return the value of the cell
     */
    virtual T getCell(const I idx) const = 0;

    /**
     * @brief Write a value into the cell
     * @param idx : an index structure
     * @param value : a value to write into the cell
     */

    virtual void setCell(const I idx, const T value) = 0;

    /**
     * @brief return the Size of the grid
     */

    virtual I getSize() const = 0;

    /**
     * @brief Create a new Grid with the 2 template values passed
     * @return Return a pointer on the grid created
     */

    virtual Grid<T, I>* createNew() const = 0;

    /**
     * @brief iterate_get
     * @param functor
     */
    virtual void iterate_get(const std::function<void(const T value)> functor) const = 0;
    virtual void iterate_get(const std::function<void(const I index, const T value)> functor) const = 0;
    /**
     * @brief iterate_set
     * @param functor
     */
    virtual void iterate_set(const std::function<T()> functor) = 0;
    virtual void iterate_set(const std::function<T(const I index)> functor) = 0;

    /**
     * @brief Save the grid attributes in a file
     * @param filePath : a string that refers to a file, where to write
     */
    virtual void save(const std::string& filePath) const = 0;

    /**
     * @brief load a Grid attributs from a file
     * @param filePath : the name of the file from which to take values for the attributs
     */
    virtual void load(const std::string& filePath) = 0;
};

/**
 * @brief The Grid1D stores a 1 dimensional grid
 */

template <typename T>
class Grid1D : public Grid<T, Index1D> {
private:
    std::vector<T> values;

public:
    Grid1D(const int size)
        : values(size, 0)
    {
    }

    virtual ~Grid1D() {}

    /**
     * @brief Get the value of a cell with a one dimensional index
     * @param idx : a one dimensional index (i.e : a structure with one interger)
     * @return the value stored in the cell
     */

    virtual T getCell(const Index1D idx) const
    {
        if (this->wrapAround)
            return values[idx.i % values.size()];
        else if (idx.i < 0 || idx.i >= static_cast<int>(values.size()))
            return T();
        else
            return values[idx.i];
    }

    /**
     * @brief set the value given in parameters in the cell
     * @param idx : index of a cell for a 1 dimensional grid
     * @param value : the value to set in the cell
     */
    virtual void setCell(const Index1D idx, const T value)
    {
        values.at(idx.i) = value;
    }

    /**
     * @brief create a new Grid1D with the same size as the current object
     * @return return a pointer on the new grid
     */

    Grid1D<T>* createNew() const
    {
        auto* newGrid = new Grid1D(values.size());
        return newGrid;
    }

    /**
     * @brief iterate_get
     * @param functor
     */

    virtual void iterate_get(const std::function<void(const T value)> functor) const
    {
        for (Index1D i; i.i < static_cast<int>(values.size()); ++i.i)
            functor(getCell(i));
    }
    virtual void iterate_get(const std::function<void(const Index1D index, const T value)> functor) const
    {
        for (Index1D i; i.i < static_cast<int>(values.size()); ++i.i)
            functor(i, getCell(i));
    }
    virtual void iterate_set(const std::function<T()> functor)
    {
        for (Index1D i; i.i < static_cast<int>(values.size()); ++i.i)
            setCell(i, functor());
    }
    virtual void iterate_set(const std::function<T(const Index1D index)> functor)
    {
        for (Index1D i; i.i < static_cast<int>(values.size()); ++i.i)
            setCell(i, functor(i));
    }

    /**
     * @brief save the current grid into a file
     * @param filePath : the name of the file where to save the grid
     */
    virtual void save(const std::string& filePath) const
    {
        try {
            std::cout << "save 1D, " << filePath << "\n";
            std::ofstream file(filePath, std::ios::out | std::ios::trunc);
            file << "1 " << values.size() << " ";
            iterate_get([&](const T cell) { file << cell << " "; });
            file.close();
        } catch (const std::string& e) {
            std::cout << "erreur: " << e << "\n";
        }
    }

    /**
     * @brief return the size of the grid
     * @return
     */

    virtual Index1D getSize() const { return values.size(); }

    /**
     * @brief load a grid configuration from a file
     * @param filePath : the name of the file where is stored the grid
     */

    virtual void load(const std::string& filePath)
    {
        try {
            std::ifstream file(filePath, std::ios::in);
            T cell;
            size_t it;
            file >> it;
            if (it != 1) {
                throw std::string("wrong loading, expected a 1D grid \n");
            }
            file >> it;
            values.resize(it, 0);
            for (Index1D i; i.i < it; ++i.i) {
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

    virtual void iterate_get(const std::function<void(const T value)> functor) const
    {
        for (Index2D i; i.row < height; ++i.row)
            for (i.col = 0; i.col < width; ++i.col)
                functor(getCell(i));
    }
    virtual void iterate_get(const std::function<void(const Index2D, const T value)> functor) const
    {
        for (Index2D i; i.row < height; ++i.row)
            for (i.col = 0; i.col < width; ++i.col)
                functor(i, getCell(i));
    }
    virtual void iterate_set(const std::function<T()> functor)
    {
        for (Index2D i; i.row < height; ++i.row)
            for (i.col = 0; i.col < width; ++i.col)
                setCell(i, functor());
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
            std::cout << "saving grid height " << height << " width " << width << "\n";
            iterate_get([&](const T cell) { file << cell << " "; });
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
            if (it != 2) {
                throw std::string("wrong loading, expected 2D grid \n");
            }
            file >> height;
            file >> width;
            std::cout << "loading grid height " << height << " width " << width << "\n";
            T cell;
            values.resize(height * width, 0);
            for (Index2D i; i.row < height; ++i.row) {
                for (i.col = 0; i.col < width; ++i.col) {
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
