#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <memory>

#include "Grid.h"
#include "History.h"
#include "Index.h"
#include "Rule.h"

template <typename T, typename I>
class Automaton {
private:
	std::unique_ptr<History<Grid<T, I>>> history;
	std::unique_ptr<Rule<T, I>> rule;

public:
	Automaton(History<Grid<T, I>>* history, Rule<T, I>* rule)
		: history(history)
		, rule(rule)
	{
	}
	Automaton(const Automaton& other) = delete;
	virtual void operator=(const Automaton& other) = delete;
	virtual ~Automaton() {}

	History<Grid<T, I>>* getHistory() { return history.get(); }
	Rule<T, I>* getRule() { return rule.get(); }

	virtual void calculate(const Grid<T, I>& old, Grid<T, I>& next) const;
	virtual void next();
	virtual void run(const int count);
};

template <typename T, typename I>
void Automaton<T, I>::next()
{
	if (this->history == nullptr)
		throw std::logic_error("history is null");

	// TODO check this
	auto lastGrid = this->history->getLast();
	auto newGrid = lastGrid->createNew();
	this->calculate(*lastGrid, *newGrid);
	this->history->push(newGrid.get());
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

// TODO fonctions de sauvegarde

#endif // AUTOMATON_H
