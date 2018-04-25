#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "Grid.h"
#include "History.h"

template<typename T, typename I>
class Automaton
{
private:
    History<Grid<T,I>> history;
public:
    Automaton(); //initialiser historique ou passer historique en pramètre
    void calculate(const Grid<T,I> &old, Grid<T,I> &next) const; //
	void next();
	void run(const int count);
};

#endif // AUTOMATON_H
