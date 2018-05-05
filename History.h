#ifndef HISTORY_H
#define HISTORY_H

#include<vector>

/**
 * @brief The History abstract class stores a history of elements (usually
 * grids, i.e. the state of an Automaton through time).
 * The first and last state are guaranteed to be stored and accessible at any
 * time, however different subclasses allow different storing strategies. For
 * instance only the last element could be stored (in addition to the first
 * one), or the n last ones, or it can even be a sparse history (every n-th one)
 * with dynamic recalculation of intermediate steps.
 */
template<typename T>
class History
{
private:
	T *start = nullptr;

public:
	History();
	virtual ~History() { delete this->start; }

	/**
	 * @brief Stores the first element, and resets the history.
	 * @attention Subclasses should clean/reset their internal history when
	 * calling this method.
	 * @param start The first element to be stored
	 */
	virtual void setStart(const T &start) { this->start = &start; }
	/**
	 * @brief Returns the first element stored in the history.
	 * @return
	 */
	virtual T *getStart() const { return this->start; }
	/**
	 * @brief Returns the last element stored in the history.
	 * @return
	 */
	virtual T *getLast() const = 0;
	/**
	 * @brief Returns the i-th element stored in the history or a nullptr if
	 * this element isn't available.
	 * @param i index of the element to retrieve
	 * @return
	 */
	virtual T *get(const int i) const = 0; // TODO peut-être tout passer en pointeur du coup ?
	/**
	 * @brief Add a new element to be stored in the history.
	 * @param newElement
	 */
	virtual void push(const T *newElement) = 0;
};

/**
 * @brief The RingHistory stores the last n elements.
 * It implements a ring buffer for memory efficiency.
 */
template<typename T>
class RingHistory : public History<T>
{
private:
	std::vector<T*> ring;
	int current = 0;

public:
	RingHistory(const int size);
	~RingHistory();

	virtual void setStart (const T &start);
	virtual T *getLast() const;
	virtual T *get(const int i) const;
	virtual void push(const T *newElement);
};

#endif // HISTORY_H
