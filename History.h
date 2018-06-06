#ifndef HISTORY_H
#define HISTORY_H

#include <memory>
#include <vector>

/**
 * @brief The History abstract class stores a history of elements (usually
 * grids, i.e. the state of an Automaton through time).
 * The first and last state are guaranteed to be stored and accessible at any
 * time, however different subclasses allow different storing strategies. For
 * instance only the last element could be stored (in addition to the first
 * one), or the n last ones, or it can even be a sparse history (every n-th one)
 * with dynamic recalculation of intermediate steps.
 */
template <typename T>
class History {
private:
	std::unique_ptr<const T> start;

public:
	History() {}
	History(const History& other) = delete;
	virtual void operator=(const History& other) = delete;
	virtual ~History() {}

	/**
	 * @brief Stores the first element, and resets the history.
	 * @attention Subclasses should clean/reset their internal history when
	 * calling this method.
	 * @param start The first element to be stored
	 */
	virtual void setStart(const T& start) { this->start.reset(&start); }
	/**
	 * @brief Returns the first element stored in the history.
	 * @return
	 */
	virtual const T* getStart() const { return this->start.get(); }
	/**
	 * @brief Returns the last element stored in the history.
	 * @return
	 */
	virtual const T* getLast() const = 0;
	/**
	 * @brief Returns the i-th element stored in the history or a nullptr if
	 * this element isn't available.
	 * @param i index of the element to retrieve
	 * @return
	 */
	virtual const T* get(const int i) const = 0; // TODO peut-être tout passer en pointeur du coup ?
	/**
	 * @brief Add a new element to be stored in the history.
	 * @param newElement
	 */
	virtual void push(const T* newElement) = 0;
};

/**
 * @brief The RingHistory stores the last n elements.
 * It implements a ring buffer for memory efficiency.
 */
template <typename T>
class RingHistory : public History<T> {
private:
	std::vector<std::unique_ptr<const T>> ring;
	int current = 0;

public:
	RingHistory(const int size)
		: ring(size, nullptr)
	{
	}
	RingHistory(const RingHistory& other) = delete;
	~RingHistory()
	{
		//		for (auto e : ring)
		//			delete e;
		//		ring.clear();
	}

	virtual void setStart(const T& start)
	{
		this->History<T>::setStart(start);
		// TODO FIXME
		//		for (auto& elem : this->ring)
		//			elem.release();
		this->ring[0].reset(&start);
		this->current = 0;
	}
	virtual const T* getLast() const
	{
		return this->ring[this->current % this->ring.size()].get();
	}
	virtual const T* get(const int i) const
	{
		if (i > this->current)
			throw new std::exception(); // TODO return nullptr instead
		// std::cout << "Cet état n'a pas été enregistré, seulement les"<< size << " derniers états on été enregristrès \n";
		if (i < this->current - this->ring.size())
			throw new std::exception(); // TODO return nullptr instead

		return this->ring[i % this->ring.size()].get();
	}
	virtual void push(const T* newElement)
	{
		// TODO
	}
};

#endif // HISTORY_H
