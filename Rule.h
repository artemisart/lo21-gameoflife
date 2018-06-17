#ifndef RULES_H
#define RULES_H

#include "Grid.h"
#include "Index.h"
#include <stdexcept>

/**
 * @file Rule.h
 * @brief Rules for an Automaton to change state
 */

/**
 * @brief Rule is a class that stores the rules to apply on a Grid1D or Grid2D, it can take a cell and calculte the next stat of every value.
 */

template <typename T, typename I>
class Rule {
public:
    Rule() {}
    virtual ~Rule() {}

    /**
     * @brief calulate the next stat of a cell
     * @param grid : the type a grid which is used
     * @param index : the index of the cell into the grid
     * @return returns the value of the cell
     */

    virtual T calcNextState(const Grid<T, I>& grid, const I index) const = 0;

    /**
     * @brief save the rules inside a file
     * @param filePath : the name of the file where to store the rules
     */
    virtual void save(const std::string& filePath) const = 0;

    /**
     * @brief load rules from a file
     * @param filePath : the name of the file where to search the rules
     */
    virtual void load(const std::string& filePath) = 0;
};

class Rule1D : public Rule<bool, Index1D> {
private:
    std::uint8_t num;

public:
    Rule1D(const std::uint8_t i = 0);
    ~Rule1D() {}

    std::uint16_t getNum() const { return num; }
    void setNum(const std::uint8_t i) { num = i; }

    virtual bool calcNextState(const Grid<bool, Index1D>& grid, const Index1D index) const;
    virtual void save(const std::string& filePath) const
    {
        try {
            std::ofstream file(filePath, std::ios::out | std::ios::app);
            int u= num;
            file << "1 " << u;
            file.close();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    virtual void load(const std::string& filePath)
    {
        try {
            std::cerr << "loading";
            std::ifstream file(filePath, std::ios::in);
            std::uint16_t it;
            std::string s;
            if (!getline(file, s)) {
                throw std::invalid_argument("No rule in this file");
            }
            file >> it;
            if (it != 1) {
                throw std::invalid_argument("wrong loading, expected a 1D rule \n");
            }
            long int temp;
            file >> temp;
            num = temp;
            std::cout<<"dans rule.h num est "<<num<<"\n";
            file.close();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            throw;
        }
    }
};

class OuterTotalisticRule2D : public Rule<bool, Index2D> {
private:
    std::uint16_t born = 255, survive = 0;
    // born represent regles quand cell dead, survive quand cell lives

public:
    OuterTotalisticRule2D(const std::uint16_t b = 255, const std::uint16_t s = 0);
    ~OuterTotalisticRule2D() {}

    std::uint16_t getBorn() const { return born; }
    std::uint16_t getSurvive() const { return survive; }
    void setBorn(const std::uint16_t b) { born = b; }
    void setSurvive(const std::uint16_t s) { survive = s; }

    virtual bool calcNextState(const Grid<bool, Index2D>& grid, const Index2D index) const;
    virtual void save(const std::string& filePath) const
    {
        try {
            std::ofstream file(filePath, std::ios::out | std::ios::app);
            file << "2 " << born;
            file << " " << survive;
            std::cerr << "saving born = " << born << "  survive = " << survive << "\n";
            file.close();
        } catch (const std::exception& e) {
            std::cerr << "erreur: " << e.what() << std::endl;
        }
    }
    virtual void load(const std::string& filePath)
    {
        try {
            std::ifstream file(filePath, std::ios::in);
            std::uint16_t it;
            std::string s;
            if (!getline(file, s)) {
                throw std::invalid_argument("No rule in this file");
            }
            file >> it;
            if (it != 2) {
                throw std::invalid_argument("wrong loading, expected a 2D rule \n");
            }
            file >> born;
            file >> survive;
            std::cerr << "loading born = " << born << "  survive = " << survive << "\n";
            file.close();
        } catch (const std::exception& e) {
            std::cerr << "erreur: " << e.what() << std::endl;
            throw;
        }
    }
};

class OuterTotalisticMultiRule2D : public Rule<uint8_t, Index2D> {
private:
    std::uint16_t born = 255, survive = 0;
    // born represent regles quand cell dead, survive quand cell lives

public:
    OuterTotalisticMultiRule2D(const std::uint16_t i = 255, const std::uint16_t j = 0);
    ~OuterTotalisticMultiRule2D() {}

    uint16_t getBorn() const { return born; }
    uint16_t getSurvive() const { return survive; }
    void setBorn(const std::uint16_t b) { born = b; }
    void setSurvive(const std::uint16_t s) { survive = s; }

    virtual uint8_t calcNextState(const Grid<uint8_t, Index2D>& grid, const Index2D index) const;
    virtual void save(const std::string& filePath) const
    {
        try {
            std::ofstream file(filePath, std::ios::out | std::ios::app);
            file << "2 " << born;
            file << " " << survive;
            std::cerr << "saving born = " << born << "  survive = " << survive << "\n";
            file.close();
        } catch (const std::exception& e) {
            std::cerr << "erreur: " << e.what() << std::endl;
        }
    }
    virtual void load(const std::string& filePath)
    {
        try {
            std::ifstream file(filePath, std::ios::in);
            std::uint16_t it;
            std::string s;
            if (!getline(file, s)) {
                throw std::invalid_argument("No rule in this file");
            }
            file >> it;
            if (it != 2) {
                throw std::invalid_argument("wrong loading, expected a 2D rule \n");
            }
            file >> born;
            file >> survive;
            std::cerr << "loading born = " << born << "  survive = " << survive << "\n";
            file.close();
        } catch (const std::exception& e) {
            std::cerr << "erreur: " << e.what() << std::endl;
            throw;
        }
    }
};

#endif // RULES_H
