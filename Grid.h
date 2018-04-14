#ifndef GRID_H
#define GRID_H

#include<vector>

template<typename T>
class Grid
{
};


template<typename T>
class Grid1D : Grid<T>
{
private:
    std::size_t size;
    std::vector<T> values;

public:
    Grid1D(std::size_t size);

    T getCell(int i);
    void setCell(int i, T value);
};


template<typename T>
class Grid2D
{
private:
    std::size_t height, width;
    std::vector<T> values;

public:
    Grid2D(std::size_t height, std::size_t width);

    T getCell(int row, int col);
    void setCell(int row, int col, T value);
};

#endif // GRID_H
