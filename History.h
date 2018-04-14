#ifndef HISTORY_H
#define HISTORY_H

#include<vector>

template<typename T>
class History
{
private:
	std::vector<T> ring;
	int current;
	int size;

public:
	History(const int size);
	void setStart(const T &start);
	T &getLast() const;
	T &get(int i) const;
};

#endif // HISTORY_H
