#ifndef RULES_H
#define RULES_H

#include "Grid.h"
#include "Index.h"
#include <cstdint> // pour std::uint16_t

template <typename T, typename I>
class Rule {
public:
	Rule() {}
	virtual ~Rule() {}

	virtual T calcNextState(const Grid<T, I>& grid, const I index) const = 0;
};

class Rule1D : public Rule<bool, Index1D> {
private:
    std::uint16_t num;

public:
    Rule1D(const std::uint16_t i = 0);
	~Rule1D() {}

    std::uint16_t getNum() const;
    void setNum(const std::uint16_t i);

	virtual bool calcNextState(const Grid<bool, Index1D>& grid, const Index1D index) const;
};

class Rule2D : public Rule<bool, Index2D> {
private:
    std::uint16_t born = 255, survive = 0;
	// born represent regles quand cell dead, survive quand cell lives

public:
    Rule2D(const std::uint16_t i = 255, const std::uint16_t j = 0);
	~Rule2D() {}

    std::uint16_t getBorn() const;
    std::uint16_t getSurvive() const;
    void setBorn(const std::uint16_t i);
    void setSurvive(const std::uint16_t i);

	virtual bool calcNextState(const Grid<bool, Index2D>& grid, const Index2D index) const;
};

// TODO fonction de sauvegarde

#endif // RULES_H
