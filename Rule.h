#ifndef RULES_H
#define RULES_H

#include "Grid.h"
#include "Index.h"
#include <cstdint> // pour std::uint16_t

template <typename T, typename I>
class Rule {
public:
	Rule() {}
	virtual ~Rule() {}

	virtual T calcNextState(const Grid<T, I>& grid, const I index) const = 0;
	virtual void save(const std::string& filePath) const = 0;
	virtual void load(const std::string& filePath) = 0;
};

class Rule1D : public Rule<bool, Index1D> {
private:
    std::uint16_t num;

public:
    Rule1D(const std::uint16_t i = 0);
	~Rule1D() {}

    std::uint16_t getNum() const;
    void setNum(const std::uint16_t i);

	virtual bool calcNextState(const Grid<bool, Index1D>& grid, const Index1D index) const;
    virtual void save(const std::string& filePath) const
    {
		try {
			std::ofstream file(filePath, std::ios::out | std::ios::app);
			file << "\n"
				 << "1 " << num;
			file.close();
		} catch (const std::string& e) {
			std::cout << "erreur: " << e << "\n";
		}
	}
    virtual void load(const std::string& filePath)
    {
		try {
			std::cout << "loading";
			std::ifstream file(filePath, std::ios::in);
			std::uint16_t it;
			std::string s;
			if (!getline(file, s)) {
				throw std::string("No rule in this file");
			};
			file >> it;
			if (it != 1) {
				throw std::string("wrong loading, expected a 1D rule \n");
			}
			file >> num;
			file.close();
		} catch (const std::string& e) {
			std::cout << "erreur: " << e << "\n";
		}
	}
};

class Rule2D : public Rule<bool, Index2D> {
private:
    std::uint16_t born = 255, survive = 0;
	// born represent regles quand cell dead, survive quand cell lives

public:
    Rule2D(const std::uint16_t i = 255, const std::uint16_t j = 0);
	~Rule2D() {}

    std::uint16_t getBorn() const;
    std::uint16_t getSurvive() const;
    void setBorn(const std::uint16_t i);
    void setSurvive(const std::uint16_t i);

	virtual bool calcNextState(const Grid<bool, Index2D>& grid, const Index2D index) const;
    virtual void save(const std::string& filePath) const
    {
		try {
			std::ofstream file(filePath, std::ios::out | std::ios::app);
			file << "\n"
				 << "2 " << born;
			file << " " << survive;
			std::cout << "saving born = " << born << "  survive = " << survive << "\n";
			file.close();
		} catch (const std::string& e) {
			std::cout << "erreur: " << e << "\n";
		}
	}
    virtual void load(const std::string& filePath)
    {
		try {
			std::ifstream file(filePath, std::ios::in);
			std::uint16_t it;
			std::string s;
			if (!getline(file, s)) {
				throw std::string("No rule in this file");
			};
			file >> it;
			if (it != 2) {
				throw std::string("wrong loading, expected a 2D rule \n");
			}
			file >> born;
			file >> survive;
			std::cout << "loading born = " << born << "  survive = " << survive << "\n";
			file.close();
		} catch (const std::string& e) {
			std::cout << "erreur: " << e << "\n";
		}
	}
};

// TODO fonction de sauvegarde

#endif // RULES_H
