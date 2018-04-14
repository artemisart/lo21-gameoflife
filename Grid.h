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
    std::vector<T> values;

public:
    Grid1D();

    T getCell(int i);
    void setCell(int i, T value);
};


template<typename T>
class Grid2D
{
private:
    std::vector<T> values;

public:
    Grid2D() {}

    T getCell(int row, int col);
    void setCell(int row, int col, T value);
};

#endif // GRID_H
