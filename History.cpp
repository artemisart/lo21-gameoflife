#include "History.h"
#include<iostream>


template<typename T>
RingHistory<T>::RingHistory(const int size) : ring(size) {}

template<typename T>
RingHistory<T>::~RingHistory()
{
	delete this->ring;
}

template<typename T>
void RingHistory<T>::setStart (const T &start)
{
	this->History<T>::setStart(start);

	for (auto &elem : this->ring)
		elem = nullptr;
	this->ring[0] = &start;
	this->current = 0;
}

template<typename T>
T* RingHistory<T>::getLast() const
{
	return this->ring[this->current % this->ring.size()];
}

template<typename T>
T* RingHistory<T>::get(const int i) const
{
	if (i > this->current)
		throw new std::exception(); // TODO
		// std::cout << "Cet état n'a pas été enregistré, seulement les"<< size << " derniers états on été enregristrès \n";
	if (i < this->current - this->ring.size())
		throw new std::exception(); // TODO

	return this->ring[i % this->ring.size()];
}

template<typename T>
void RingHistory<T>::push(const T *newElement)
{
	// TODO
}
