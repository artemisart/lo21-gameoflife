#ifndef RULES_H
#define RULES_H

#include <cstdint>
#include "Grid.h"

template<typename T, typename I>
class Rule
{
public:
	Rule();
	virtual ~Rule() = 0;
	virtual T calcNextState(const Grid<T,I> &grid, const I index) const = 0;
};


class Rule1D : public Rule<bool, Index1D>
{
private:
	std::uint8_t num;

public:
	Rule1D(const std::uint8_t num);
};


class Rule2D : public Rule<bool, Index2D>
{
private:
	std::uint8_t born, survive;

public:
	Rule2D(const std::uint8_t born, const std::uint8_t survive);
};

#endif // RULES_H
