#include "welcome.h"
#include "automate_1d.h"
#include "automate_2d.h"
#include "ui_welcome.h"

Welcome::Welcome(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Welcome)
{
    ui->setupUi(this);
    connect(ui->Automate_1D, SIGNAL(clicked()), this, SLOT(Simulation_Automaton1D()));
    connect(ui->Automate_2D, SIGNAL(clicked()), this, SLOT(Simulation_Automaton2D()));
    connect(ui->Automate_Multiple_States, SIGNAL(clicked()), this, SLOT(Simulation_Automaton_Multiple_States()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(Quit()));
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::Simulation_Automaton1D()
{
    Automate_1D* simulator = new Automate_1D;
    simulator->setParent(this);
    this->hide();
    simulator->show();
    simulator->auto_load();
}

void Welcome::Simulation_Automaton2D()
{
    Automate_2D* simulator = new Automate_2D;
    simulator->setParent(this);
    // simulator->setType(0);
    this->hide();
    simulator->show();
}

void Welcome::Simulation_Automaton_Multiple_States()
{
    Automate_2D* simulator = new Automate_2D;
    simulator->setParent(this);
    //simulator->setType(1);
    this->hide();
    simulator->show();
}

void Welcome::Quit()
{
    this->close();
}
