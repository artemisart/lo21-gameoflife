#include "Automaton.h"

template<typename T, typename I>
Automaton<T,I>::Automaton()
{

}

template<typename T, typename I>
void Automaton<T,I>::calculate(const Grid<T, I> &old, Grid<T, I> &next) const
{
	for (I index : old)
	{
		next.setCell(index, this->calcNextState(old, index));
	}
}
