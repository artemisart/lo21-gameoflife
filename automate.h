#ifndef AUTOMATE
#define AUTOMATE
/*
#include <string>
#include <iostream>

class AutomateException {
public:
    AutomateException(const std::string& message) :info(message) {}
    std::string getInfo() const { return info; }
private:
    std::string info;
};

short unsigned int NumBitToNum(const std::string& num);
std::string NumToNumBit(short unsigned int num);

class Etat {
    unsigned int dimension;
    bool* valeur;
public:
    Etat():dimension(0), valeur(nullptr) {} //valeurs par défaut
    Etat(unsigned int n);
    ~Etat() { delete[] valeur; }
    Etat(const Etat& e);
    Etat& operator=(const Etat& e);
    void setCellule(unsigned int i, bool val);
    bool getCellule(unsigned int) const;
    unsigned int getDimension() const { return dimension; }
};

std::ostream& operator<<(std::ostream& f, const Etat& e);

class Automate {
    unsigned short int numero;
    std::string numeroBit;

    Automate(const Automate& a) = default;
    Automate& operator=(const Automate& a) = default;
    friend class AutomateManager;
public:
    Automate(unsigned short int num);
    Automate(const std::string& num);
    ~Automate() = default;
    unsigned short int getNumero() const { return numero; }
    const std::string& getNumeroBit() const { return numeroBit; }
    void appliquerTransition(const Etat& dep, Etat& dest) const;
};

std::ostream& operator<<(std::ostream& f, const Automate& t);

class Simulateur {
    const Automate& automate;
    Etat** etats=nullptr;
    const Etat* depart=nullptr;
    unsigned int nbMaxEtats;
    unsigned int rang=0;
    void build(unsigned int c);
    Simulateur(const Simulateur& s) = delete;
    Simulateur& operator=(const Simulateur& s) = delete;
public:
    Simulateur(const Automate& a, unsigned int buffer = 2);
    Simulateur(const Automate& a, const Etat& dep, unsigned int buffer = 2);
    void setEtatDepart(const Etat& e);
    void run(unsigned int nbSteps); // génère les n prochains états
    void next(); // génère le prochain état
    const Etat& dernier() const;
    unsigned int getRangDernier() const { return rang; }
    void reset(); // revenir à l'état de départ
    ~Simulateur();
    friend class Iterator;
    class Iterator {
        friend class Simulateur;
        Simulateur* sim = nullptr;
        int i = 0;
        Iterator(Simulateur* s):sim(s), i(s->rang){}
    public:
        Iterator() {}
        bool isDone() const {
            return sim == nullptr ||
                   (i == -1 && sim->rang<sim->nbMaxEtats) ||
                   i == sim->rang - sim->nbMaxEtats;
        }
        void next() {
            if (isDone())
                throw AutomateException("error, next on an iterator which is done");
            i--;
            if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
        }
        Etat& current() const {
            if (isDone())
                throw AutomateException("error, indirection on an iterator which is done");
            return *sim->etats[i%sim->nbMaxEtats];
        }
    };
    Iterator getIterator() {
        return Iterator(this);
    }
    class ConstIterator {
        friend class Simulateur;
        const Simulateur* sim = nullptr;
        int i = 0;
        ConstIterator(const Simulateur* s) :sim(s), i(s->rang) {}
    public:
        ConstIterator() {}
        bool isDone() const {
            return sim==nullptr || (i == -1 && sim->rang<sim->nbMaxEtats) ||
                i== sim->rang - sim->nbMaxEtats;
        }
        void next() {
            if (isDone())
                throw AutomateException("error, next on an iterator which is done");
            i--;
            if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
        }
        const Etat& current() const {
            if (isDone())
                throw AutomateException("error, indirection on an iterator which is done");
            return *sim->etats[i%sim->nbMaxEtats];
        }
    };
    ConstIterator getIterator() const{
        return ConstIterator(this);
    }
    ConstIterator getConstIterator() const {
        return ConstIterator(this);
    }

    class iterator {
        friend class Simulateur;
        Simulateur* sim = nullptr;
        int i = 0;
        iterator(Simulateur* s) :sim(s), i(s->rang) {}
        iterator(Simulateur* s, int dep) :sim(s), i(dep) {}
    public:
        iterator() {}
        iterator& operator++() {
            i--;
            if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
            return *this;
        }
        Etat& operator*() const {
            return *sim->etats[i%sim->nbMaxEtats];
        }
        bool operator!=(iterator it) const { return sim != it.sim || i != it.i; }
    };

    iterator begin() {	return iterator(this); }
    iterator end() { if (rang < nbMaxEtats) return iterator(this, -1); else return iterator(this, rang - nbMaxEtats);  }

    class const_iterator {
        friend class Simulateur;
        const Simulateur* sim = nullptr;
        int i = 0;
        const_iterator(const Simulateur* s) :sim(s), i(s->rang) {}
        const_iterator(const Simulateur* s, int dep) :sim(s), i(dep) {}
    public:
        const_iterator() {}
        const_iterator& operator++() {
            i--;
            if (i == -1 && sim->rang >= sim->nbMaxEtats) i = sim->nbMaxEtats - 1;
            return *this;
        }
        const Etat& operator*() const {
            return *sim->etats[i%sim->nbMaxEtats];
        }
        bool operator!=(const_iterator it) const { return sim != it.sim || i != it.i; }
    };

    const_iterator begin() const { return const_iterator(this); }
    const_iterator end() const { if (rang < nbMaxEtats) return const_iterator(this, -1); else return const_iterator(this, rang - nbMaxEtats); }
    const_iterator cbegin() const { return const_iterator(this); }
    const_iterator cend() const { if (rang < nbMaxEtats) return const_iterator(this, -1); else return const_iterator(this, rang - nbMaxEtats); }

};

class AutomateManager {
    Automate* automates[256];
    AutomateManager();
    ~AutomateManager();
    AutomateManager(const AutomateManager& a) = delete;
    AutomateManager& operator=(const AutomateManager& a) = delete;
    struct Handler {
        AutomateManager* instance;
        Handler():instance(nullptr) {}
        ~Handler() { delete instance;  }
    };
    static Handler handler;
public:
    const Automate& getAutomate(unsigned short int num);
    const Automate& getAutomate(const std::string& num);
    static AutomateManager& getAutomateManager();
    static void libererAutomateManager();
};
*/
#endif // AUTOMATE

