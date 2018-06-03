#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>

namespace Ui {
class Welcome;
}

class Welcome : public QWidget
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();

private:
    Ui::Welcome *ui;

private slots:
    void Simulation_Automaton1D();
    void Simulation_Automaton2D();
    void Simulation_AutomatonHexa();
    void Quit();


};

#endif // WELCOME_H
