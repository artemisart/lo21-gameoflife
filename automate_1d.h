#ifndef AUTOMATE_1D_H
#define AUTOMATE_1D_H

#include "Rule.h"
#include <Automaton.h>
#include <QIntValidator>
#include <QModelIndex>
#include <QString>
#include <QTableWidget>
#include <QWidget>

namespace Ui {
class Automate_1D;
}

class Automate_1D : public QWidget {
    Q_OBJECT

public:
	explicit Automate_1D(QWidget* parent = nullptr);
    ~Automate_1D();
	void setParent(QWidget* par) { parent = par; }
	void inc_Rang() { rang++; }
	int getRang() const { return rang; }
    void init_simulation();

private:
	Ui::Automate_1D* ui;
    QWidget* parent;
    QIntValidator* zeroOneValidator;
    Automaton<bool, Index1D>* a;
    Rule1D* r;
    RingHistory<Grid<bool, Index1D>>* h;
    Grid<bool, Index1D>* start;
    int rang;
    QTableWidget* etats;
    bool sim;

private slots:
	void setSize();
	void synchronizeNumToNumBit(int j);
	void synchronizeNumBitToNum(const QString& s);
	void simulation();
	void cellActivation(const QModelIndex& index);
	void next();
	void menu();
};

QString NumToNumBit(short unsigned int num);
short unsigned int NumBitToNum(const QString& num);

#endif // AUTOMATE_1D_H
