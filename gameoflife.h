#ifndef GAMEOFLIFE
#define GAMEOFLIFE

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>
#include<QSpinBox>
#include<QIntValidator>


class Gameoflife : public QWidget {
    Q_OBJECT
    QHBoxLayout* presentation, *numeroc, *param, *wrapper;
    QLabel* introduction, *numl, *numeroBitl[8];
    QVBoxLayout* presentation2, *colone1, *colone2, *grid, *numc, *bitc[8];
	QWidget* Simulation;
    QPushButton * Automaton1D, *Automaton2D, *HexaAutomaton, *retour,* runSimulation, *next, *size;
    QLineEdit *colField, *rowField, *numeroBit[8];
	QTableWidget* depart;
    QSpinBox *num;
    QIntValidator *zeroOneValidator;
    static unsigned int dimension;

public:
	explicit Gameoflife(QWidget* parent = nullptr);
	QWidget* createNewWindow();
    //~Gameoflife(){}
private slots:
    void Simulation_Automaton1D();
    void Simulation_Automaton2D();
    void Simulation_HexaAutomaton();
    void retour_menu();
    void setGrid1D();
    void setGrid2D();
    void synchronizeNumToNumBit(int i);
    void synchronizeNumBitToNum(const QString& s);
};

#endif // GAMEOFLIFE
