#include "Rule.h"
#include <limits>

/*--------------------------------RULE 1D-------------------------------------*/

Rule1D::Rule1D(const std::uint8_t i)
    : num(i)
{
}

bool Rule1D::calcNextState(const Grid<bool, Index1D>& grid, const Index1D index) const
{
    int y = +4 * grid.getCell(index.offset(-1))
        + 2 * grid.getCell(index)
        + 1 * grid.getCell(index.offset(+1));
    return (this->num >> y) & 1;
}

/*--------------------------------RULE 2D-------------------------------------*/

OuterTotalisticRule2D::OuterTotalisticRule2D(const std::uint16_t b, const std::uint16_t s)
    : born(b)
    , survive(s)
{
}

bool OuterTotalisticRule2D::calcNextState(const Grid<bool, Index2D>& grid, const Index2D index) const
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

OuterTotalisticMultiRule2D::OuterTotalisticMultiRule2D(const uint16_t b, const uint16_t s)
    : born(b)
    , survive(s)
{
}

uint8_t OuterTotalisticMultiRule2D::calcNextState(const Grid<uint8_t, Index2D>& grid, const Index2D index) const
{
    int neighbourCount = +(grid.getCell(index.offset(-1, -1)) > 0)
        + (grid.getCell(index.offset(-1, 0)) > 0)
        + (grid.getCell(index.offset(-1, +1)) > 0)
        + (grid.getCell(index.offset(0, -1)) > 0)
        + (grid.getCell(index.offset(0, +1)) > 0)
        + (grid.getCell(index.offset(+1, -1)) > 0)
        + (grid.getCell(index.offset(+1, 0)) > 0)
        + (grid.getCell(index.offset(+1, +1)) > 0);

    auto current = grid.getCell(index);
    auto rule = (current > 0) ? this->survive : this->born;
    auto live = (rule >> neighbourCount) & 1;
    if (!live)
        return 0;
    if (current < std::numeric_limits<uint8_t>::max()) // avoid overflow
        return current + 1;
    return current;
}
