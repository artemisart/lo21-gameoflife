#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "Grid.h"
#include "History.h"
#include "Rule.h"
#include "Index.h"

template<typename T, typename I>
class Automaton
{
private:
    History<Grid<T,I>> history;
	Rule<T,I> *rule;
	// Rule<typename G::EtatType, typename G::IndexType> *rule;
public:
    Automaton(); //initialiser historique ou passer historique en pramètre
	virtual void calculate(const Grid<T,I> &old, Grid<T,I> &next) const; //
	virtual void next();
	virtual void run(const int count);
};

template<typename T, typename I>
void Automaton<T,I>::next()
{
	T lastGrid = this->history.getLast();
	// crée une nouvelle grille avec la même taille que les précédentes
	T *newGrid = new T(lastGrid);
	this->calculate(lastGrid, newGrid);
	this->history.push(newGrid);
}

template<typename T, typename I>
void Automaton<T,I>::run(const int count)
{
	for (int i = 0; i < count; ++i)
		this->next();
}

/* quand on aura un pattern iterator sur la grid ça devrait marcher
template<typename T, typename I>
void Automaton<T,I>::calculate(const Grid<T, I> &old, Grid<T, I> &next) const
{
	// http://en.cppreference.com/w/cpp/language/range-for
	for (I index : old)
	{
		next.setCell(index, this->calcNextState(old, index));
	}
}//*/

// TODO remplacer par un visitor sur la grid
//template<typename T>
//void Automaton<T,Index1D>::calculate(const Grid<T,Index1D> &old, Grid<T,Index1D> &next) const
//{
//	for (int i = 0; i < old.size; ++i)
//		next.setCell(i, rule->calcNextState(old, i));
//}

//template<typename T>
//void Automaton<T,Index2D>::calculate(const Grid<T,Index2D> &old, Grid<T,Index2D> &next) const
//{
//	for (int row = 0; i < old.height; ++i)
//		for (int col = 0; i < old.width; ++i)
//			next.setCell(i, rule->calcNextState(old, i));
//}

#endif // AUTOMATON_H
