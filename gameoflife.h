#ifndef GAMEOFLIFE
#define GAMEOFLIFE

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>

class Gameoflife : public QWidget {
    Q_OBJECT
	QPushButton* Automaton1D;
	QPushButton* Automaton2D;
	QPushButton* HexaAutomaton;
	QHBoxLayout* presentation;
	QLabel* introduction;
	QVBoxLayout* presentation2;
	QWidget* Simulation;
	QPushButton* retour;
	QPushButton* runSimulation;
	QPushButton* next;
	QPushButton* size;
    QVBoxLayout *colone1, *grid;
    QLineEdit *colField, *rowField;
	QHBoxLayout* param;
	QTableWidget* depart;
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
};

#endif // GAMEOFLIFE
