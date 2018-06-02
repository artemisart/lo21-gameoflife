
#include "automate.h"
/*

Etat::Etat(unsigned int n):
    dimension(n),
    valeur(new bool [n]) {
    for (unsigned int i = 0; i < n; i++) valeur[i] = false;
}

void Etat::setCellule(unsigned int i, bool val) {
    if (i >= dimension) throw AutomateException("Erreur Cellule inexistante");
    valeur[i] = val;
}

bool Etat::getCellule(unsigned int i) const {
    if (i >= dimension) throw AutomateException("Erreur Cellule inexistante");
    return valeur[i];
}

Etat::Etat(const Etat& e):
    dimension(e.dimension),
    valeur(new bool[e.dimension])
{
    for (unsigned int i = 0; i < dimension; i++) valeur[i] = e.valeur[i];
}

std::ostream& operator<<(std::ostream& f, const Etat& e) {
    for (unsigned int i = 0; i < e.getDimension(); i++)
        if (e.getCellule(i)) f << char(178); else f << " "; // ???
    return f;
}

Etat& Etat::operator=(const Etat& e) {
    if (this != &e) {
        if (dimension != e.dimension) {
            bool* newvaleur = new bool[e.dimension];
            for (unsigned int i = 0; i < e.dimension; i++) newvaleur[i] = e.valeur[i];
            bool* old = valeur;
            valeur = newvaleur;
            dimension = e.dimension;
            delete[] old;
        }else for (unsigned int i = 0; i < e.dimension; i++) valeur[i] = e.valeur[i];
    }
    return *this;
}




void Automate::appliquerTransition(const Etat& dep, Etat& dest) const {
    if (dep.getDimension() != dest.getDimension()) dest = dep;
    for (unsigned int i = 0; i < dep.getDimension(); i++) {
        unsigned short int conf=0;
        if (i > 0) conf+=dep.getCellule(i - 1) * 4;
        conf+=dep.getCellule(i)*2;
        if (i < dep.getDimension()-1) conf+=dep.getCellule(i + 1);
        dest.setCellule(i, numeroBit[7-conf]-'0');
    }
}

std::ostream& operator<<(std::ostream& f, const Automate& A) {
    f << A.getNumero() << " : " << A.getNumeroBit() << "\n";
    return f;
}


Simulateur::Simulateur(const Automate& a, unsigned int buffer):
    automate(a), nbMaxEtats(buffer) {
    etats = new Etat*[nbMaxEtats];
    for (unsigned int i = 0; i < nbMaxEtats; i++) etats[i] = nullptr;
}
Simulateur::Simulateur(const Automate& a, const Etat& dep, unsigned int buffer):
    automate(a), depart(&dep), nbMaxEtats(buffer) {
    etats = new Etat*[nbMaxEtats];
    for (unsigned int i = 0; i < nbMaxEtats; i++) etats[i] = nullptr;
    etats[0] = new Etat(dep);
}

void Simulateur::build(unsigned int cellule) {
    if (cellule >= nbMaxEtats) throw AutomateException("erreur taille buffer");
    if (etats[cellule] == nullptr) etats[cellule] = new Etat;
}

void Simulateur::setEtatDepart(const Etat& e) {
    depart = &e;
    reset();
}

void Simulateur::reset() {
    if (depart==nullptr) throw AutomateException("etat depart indefini");
    build(0); *etats[0] = *depart;
    rang = 0;
}

void Simulateur::next() {
    if (depart == nullptr) throw AutomateException("etat depart indefini");
    rang++;
    build(rang%nbMaxEtats);
    automate.appliquerTransition(*etats[(rang - 1) % nbMaxEtats], *etats[rang%nbMaxEtats]);
}

void Simulateur::run(unsigned int nb_steps) {
    for (unsigned int i = 0; i < nb_steps; i++) next();
}

const Etat& Simulateur::dernier() const {
    return *etats[rang%nbMaxEtats];
}

Simulateur::~Simulateur() {
    for (unsigned int i = 0; i < nbMaxEtats; i++) delete etats[i];
    delete[] etats;
}

AutomateManager::AutomateManager() {
    for (unsigned int i = 0; i < 256; i++) automates[i] = nullptr;
}

AutomateManager::~AutomateManager() {
    for (unsigned int i = 0; i < 256; i++) delete automates[i];
}

AutomateManager::Handler AutomateManager::handler = Handler();

AutomateManager& AutomateManager::getAutomateManager() {
    if (!handler.instance) handler.instance = new AutomateManager;
    return *handler.instance;
}

void AutomateManager::libererAutomateManager() {
    delete handler.instance;
    handler.instance = nullptr;
}

const Automate& AutomateManager::getAutomate(unsigned short int num) {
    if (!automates[num]) automates[num] = new Automate(num);
    return *automates[num];
}

*/
