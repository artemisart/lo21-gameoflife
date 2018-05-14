#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "Grid.h"
#include "History.h"
#include "Index.h"
#include "Rule.h"

template <typename T, typename I>
class Automaton {
private:
	History<Grid<T, I>>* history = nullptr;
	Rule<T, I>* rule = nullptr;
	// Rule<typename G::EtatType, typename G::IndexType> *rule;
public:
	Automaton() {} //initialiser historique ou passer historique en pramètre
	virtual ~Automaton() {}

	virtual void calculate(const Grid<T, I>& old, Grid<T, I>& next) const; //
	virtual void next();
	virtual void run(const int count);
};

template <typename T, typename I>
void Automaton<T, I>::next()
{
	if (!this->history)
		throw std::logic_error("history is null");
	auto* lastGrid = this->history->getLast();
	// crée une nouvelle grille avec la même taille que les précédentes
	//	auto *newGrid = new Grid<T,I>(lastGrid); // FIXME Grid est abstrait donc c'est pas possible là
	//	this->calculate(lastGrid, newGrid);
	//	this->history->push(newGrid);
}

template <typename T, typename I>
void Automaton<T, I>::run(const int count)
{
	for (int i = 0; i < count; ++i)
		this->next();
}

template <typename T, typename I>
void Automaton<T, I>::calculate(const Grid<T, I>& old, Grid<T, I>& next) const
{
	next.iterate_set([&](I idx) { return rule->calcNextState(old, idx); });
}

#endif // AUTOMATON_H
