#ifndef INDEX_H
#define INDEX_H

struct Index
{
	Index() {}
};


struct Index1D : public Index
{
	int i;
	Index1D(const int i) : i(i) {}
    Index1D() { i = 0; }
};


struct Index2D : public Index
{
	int row, col;
	Index2D(const int row, const int col) : row(row), col(col) {}
    Index2D() { row = 0; col = 0; }
};

#endif // INDEX_H
