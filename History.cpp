#include "History.h"
#include<iostream>


template<typename T>
History<T>::History(const int size) : ring(size) {}

template<typename T>
History<T>::~History()
{
	delete ring;
	delete start;
}

template<typename T>
void History<T>::setStart (const T &start)
{
	this->start = &start;
	for (auto &elem : this->ring)
		elem = nullptr;
	this->ring[0] = &start;
	this->current = 0;
}

template<typename T>
T& History<T>::getLast() const
{
	return this->ring[this->current % this->ring.size()];
}

template<typename T>
T& History<T>::get(const int i) const
{
	if (i > this->current)
		throw new std::exception(); // TODO
		// std::cout << "Cet état n'a pas été enregistré, seulement les"<< size << " derniers états on été enregristrès \n";
	if (i < this->current - this->ring.size())
		throw new std::exception(); // TODO

	return this->ring[i % this->ring.size()];
}
