#ifndef INDEX_H
#define INDEX_H

/**
 * @brief The Index struct is used to represent a position (index) in n-dimensional space.
 * Note that this struct doesn't implement anything and that you should use a
 * subclass to do actual work.
 */
struct Index
{
	Index() {}
};

/**
 * @brief The Index1D struct is used to represent a position (index) in 1 dimensional space, such as an array of values.
 */
struct Index1D : public Index
{
	int i;

	Index1D(const int i) : i(i) {}
    Index1D() { i = 0; }

	/**
	 * @brief Returns a new Index1D with a given offset from the current one
	 * @param i Offset from current position
	 * @return
	 */
	Index1D offset(const int di) { return Index1D(this->i + di); }
};

/**
 * @brief The Index2D struct is used to represent a position (index) in 2 dimensional space, such as an 2d-grid of values.
 */
struct Index2D : public Index
{
	int row, col;

	Index2D(const int row, const int col) : row(row), col(col) {}
    Index2D() { row = 0; col = 0; }

	/**
	 * @brief Returns a new Index2D with a given offset from the current one
	 * @param drow Column offset from current position
	 * @param dcol Row offset from current position
	 * @return
	 */
	Index2D offset(const int drow, const int dcol)
	{
		return Index2D(this->row + drow, this->col + dcol);
	}
};

#endif // INDEX_H
