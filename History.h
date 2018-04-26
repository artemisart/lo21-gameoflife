#ifndef HISTORY_H
#define HISTORY_H

#include<vector>

template<typename T>
class History
{
private:
    std::vector<T> ring; //ajouter état départ
	int current;
	int size;

public:
	History(const int size);
    ~History();
    void setStart (const T &start);
	T &getLast() const;
    T &get(int i) const; //retourner en arrière
};

#endif // HISTORY_H
