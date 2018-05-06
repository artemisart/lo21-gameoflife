#ifndef GAMEOFLIFE
#define GAMEOFLIFE

#include<QWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QLabel>

class Gameoflife : public QWidget{
    Q_OBJECT
    QPushButton *Automaton1D;
    QPushButton *Automaton2D;
    QPushButton *HexaAutomaton;
    QHBoxLayout *presentation;
    QLabel * introduction;
    QVBoxLayout *presentation2;
    QWidget * Simulation;
    QPushButton * retour;
    QPushButton * runSimulation;
    QPushButton * next;
    QVBoxLayout * colone1;

public :
    explicit Gameoflife(QWidget* parent = nullptr);
    QWidget* createNewWindow();
    //~Gameoflife(){}
private slots :
    void Simulation_Automaton1D();
    void Simulation_Automaton2D();
    void Simulation_HexaAutomaton();
    void retour_menu();


};

#endif // GAMEOFLIFE

