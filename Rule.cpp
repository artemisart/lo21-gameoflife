#include "Rule.h"

/*--------------------------------RULE 1D-------------------------------------*/

Rule1D::Rule1D(const std::uint8_t i)
    : num(i)
{
}

std::uint16_t Rule1D::getNum() const
{
    return this->num;
}

void Rule1D::setNum(const std::uint8_t i)
{
    this->num = i;
}

bool Rule1D::calcNextState(const Grid<bool, Index1D>& grid, const Index1D index) const
{
    int y = +4 * grid.getCell(index.offset(-1))
        + 2 * grid.getCell(index)
        + 1 * grid.getCell(index.offset(+1));
    return (this->num >> y) & 1;
}

/*--------------------------------RULE 2D-------------------------------------*/

Rule2D::Rule2D(const std::uint16_t b, const std::uint16_t s)
    : born(b)
    , survive(s)
{
}

std::uint16_t Rule2D::getBorn() const
{
    return this->born;
}

std::uint16_t Rule2D::getSurvive() const
{
    return this->survive;
}

void Rule2D::setBorn(const std::uint16_t i)
{
    this->born = i;
}

void Rule2D::setSurvive(const std::uint16_t i)
{
    this->survive = i;
}

bool Rule2D::calcNextState(const Grid<bool, Index2D>& grid, const Index2D index) const
{
    int neighbourCount = +grid.getCell(index.offset(-1, -1))
        + grid.getCell(index.offset(-1, 0))
        + grid.getCell(index.offset(-1, +1))
        + grid.getCell(index.offset(0, -1))
        + grid.getCell(index.offset(0, +1))
        + grid.getCell(index.offset(+1, -1))
        + grid.getCell(index.offset(+1, 0))
        + grid.getCell(index.offset(+1, +1));

    auto rule = grid.getCell(index) ? this->survive : this->born;
    return (rule >> neighbourCount) & 1;
}
