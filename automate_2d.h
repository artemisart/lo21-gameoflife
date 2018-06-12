#ifndef AUTOMATE_2D_H
#define AUTOMATE_2D_H

#include "Rule.h"
#include <Automaton.h>
#include <QIntValidator>
#include <QLineEdit>
#include <QModelIndex>
#include <QString>
#include <QTableWidget>
#include <QWidget>

namespace Ui {
class Automate_2D;
}

class Automate_2D : public QWidget {
    Q_OBJECT

public:
	explicit Automate_2D(QWidget* parent = nullptr);
	~Automate_2D();
	void setParent(QWidget* par) { parent = par; }
    void incRang() { rang++; }
    int getRang() const { return rang; }
    void init_simulation();
 /*   void setType(int a){type=a;}
    const int getType()const { return type; }*/

private:
	Ui::Automate_2D* ui;
    QWidget* parent;
    QIntValidator* zeroOneValidator;
    Automaton<bool, Index2D>* a;
    Rule2D* r;
    RingHistory<Grid<bool, Index2D>>* h;
    Grid<bool, Index2D>* start;
    int rang;
    QTableWidget* etats;
    bool sim;
    QLineEdit* survive[9], *born[9];
    QTimer *timer;
    int type; // type=0 => bool, type = 1=>int

private slots:
	void setSize();
    void synchronizeNumToNumBit_b(int j);
    void synchronizeNumBitToNum_b(const QString& s);
    void synchronizeNumToNumBit_s(int j);
    void synchronizeNumBitToNum_s(const QString& s);
	void simulation();
	void cellActivation(const QModelIndex& index);
	void next();
	void menu();
    void reset();
    void stop();
    void run();
    void load();
    void save();

};

QString NumToNumBit(short unsigned int num);
short unsigned int NumBitToNum(const std::string& num);

#endif // AUTOMATE_2D_H
