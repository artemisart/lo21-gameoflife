#ifndef HISTORY_H
#define HISTORY_H

#include<vector>

template<typename T>
class History
{
private:
	std::vector<T> ring; // ajouter état départ
	T *start = nullptr;
	int current = 0;

public:
	History(const int size);
    ~History();
    void setStart (const T &start);
	T &getLast() const;
	T &get(const int i) const; // retourner en arrière
	void push(const T &newElement);
};

#endif // HISTORY_H
