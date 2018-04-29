#ifndef RULES_H
#define RULES_H

#include <cstdint>
#include "Grid.h"

template<typename T, typename I>
class Rule
{
public:
	virtual Rule();
	virtual ~Rule() = 0;
	virtual T calcNextState(const Grid<T,I> &grid, const I index) const = 0;
};


class Rule1D : public Rule<bool, Index1D>
{
private:
	std::uint8_t num;

public:
	Rule1D();
	~Rule1D();
	Rule1D(const std::uint8_t i);
	std::uint8_t getnum();
	void setnum(const std::uint8_t i);
	bool calcNextState(const Grid1D<bool,Index1D> &grid, const Index1D index) const;
};


class Rule2D : public Rule<bool, Index2D>
{
private:
	std::uint8_t born, survive; //born represente regles quand cell dead, survive quand cell lives 

public:
	Rule2D();
	~Rule2D();
	Rule2D(const std::uint8_t i, const std::uint8_t j);
	std::uint8_t getborn();
	std::uint8_t getsurvive();
	void setborn(const std::uint8_t i);
	void setsurvive(const std::uint8_t i);
	bool calcNextState(const Grid2D<bool,Index2D> &grid, const Index2D index) const;
};

#endif // RULES_H
