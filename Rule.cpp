#include "Rule.h"

/*--------------------------------RULE 1D-------------------------------------*/

Rule1D::Rule1D(const std::uint8_t i) : num(i) {}

std::uint8_t Rule1D::getNum() const
{
	return this->num;
}

void Rule1D::setNum(const std::uint8_t i)
{
	this->num = i;
}

bool Rule1D::calcNextState(const Grid<bool,Index1D> &grid, const Index1D index) const
{
	Index1D idx = index;
	idx.i--;
	int x = grid.getCell(idx)*4;
	idx.i++;
	x += grid.getCell(idx)*2;
	idx.i++;
	x += grid.getCell(idx);
	bool newVal = (this->num >> x) & 1;
	return newVal; 
}

/*--------------------------------RULE 2D-------------------------------------*/


Rule2D::Rule2D(const std::uint8_t b, const std::uint8_t s) :
	born(b),
	survive(s)
{}

std::uint8_t Rule2D::getBorn() const
{
	return this->born;
}

std::uint8_t Rule2D::getSurvive() const
{
	return this->survive;
}

void Rule2D::setBorn(const std::uint8_t i)
{
	this->born = i;
}

void Rule2D::setSurvive(const std::uint8_t i)
{
	this->survive = i;
}

bool Rule2D::calcNextState(const Grid<bool,Index2D> &grid, const Index2D index) const
{
	Index2D idx = index;
	int voisins = 0;
		idx.row--; idx.col--;
		voisins = voisins + grid.getCell(idx);
		idx.row++;
		voisins = voisins + grid.getCell(idx);
		idx.row++;
		voisins = voisins + grid.getCell(idx);
		idx.col++;
		voisins = voisins + grid.getCell(idx);
		idx.row--; idx.row--;
		voisins = voisins + grid.getCell(idx);
		idx.col++;
		voisins = voisins + grid.getCell(idx);
		idx.row++;
		voisins = voisins + grid.getCell(idx);
		idx.row++;
		voisins = voisins + grid.getCell(idx);

	if (grid.getCell(index)) // the current cell is alive
		return (this->survive >> voisins) & 1;
	else // the current cell is dead
		return (this->born >> voisins) & 1;
}
