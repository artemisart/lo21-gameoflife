#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
/**
 * @file History.h
 * @brief History made of the n last states of an Automaton
 */

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
    T* start = nullptr;

public:
    History() {}
    virtual ~History() { delete this->start; }

    /**
     * @brief Stores the first element, and resets the history.
     * @attention Subclasses should clean/reset their internal history when
     * calling this method.
     * @param start The first element to be stored
     */
    virtual void setStart(T* start) { this->start = start; }
    /**
     * @brief Returns the first element stored in the history.
     * @return
     */
    virtual T* getStart() const { return this->start; }
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
    virtual const T* get(const int i) const = 0;
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
    std::vector<const T*> ring;
    int current = 0;

public:
    RingHistory(const int size)
        : ring(size, nullptr)
    {
    }
    ~RingHistory()
    {
        for (auto e : ring)
            delete e;
        ring.clear();
    }

    virtual std::size_t size() const
    {
        return ring.size();
    }
    virtual void setStart(T* start)
    {
        this->History<T>::setStart(start);

        for (auto& elem : this->ring)
            elem = nullptr;
        this->ring[0] = start;
        this->current = 0;
    }
    virtual const T* getLast() const
    {
        return this->ring[this->current % this->ring.size()];
    }
    virtual const T* get(const int i) const
    {
        if (i < 0)
            return nullptr;
        if (i > current) // we can't get the future
            return nullptr;
        if (i < current - static_cast<int>(ring.size())) // too old, we don't have it anymore
            return nullptr;

        return this->ring[i % this->ring.size()];
    }
    virtual void push(const T* newElement)
    {
        auto& elem = ring[++current % ring.size()];
        if (elem != nullptr && elem != this->getStart())
            delete elem;
        elem = newElement;
    }
};

#endif // HISTORY_H
