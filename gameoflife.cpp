#include"Automaton.h"
#include"gameoflife.h"
#include"Rule.h"
#include"Grid.h"
#include"History.h"

Gameoflife::Gameoflife (QWidget* parent) : QWidget(parent) {

    this->setFixedSize(600,300);

    introduction = new QLabel("Bienvenue dans notre simulateur d'automate, choisissez l'automate que vous souhaitez simuler : ");

    Automaton1D = new QPushButton("Automate 1D");
    Automaton2D = new QPushButton("Automate 2D");
    HexaAutomaton= new QPushButton("Automate Hexadécimal");

    connect(Automaton1D, SIGNAL(clicked()),this, SLOT(Simulation_Automaton1D()) );
    connect(Automaton2D, SIGNAL(clicked()),this, SLOT(Simulation_Automaton2D()) );
    connect(HexaAutomaton, SIGNAL(clicked()),this, SLOT(Simulation_HexaAutomaton()) );


    presentation=new QHBoxLayout();
    presentation->addWidget(Automaton1D);
    presentation->addWidget(Automaton2D);
    presentation->addWidget(HexaAutomaton);

    presentation2 = new QVBoxLayout();
    presentation2->addWidget(introduction);
    presentation2->addLayout(presentation);

    setLayout(presentation2);

}


void Gameoflife::Simulation_Automaton1D(){
  Simulation=createNewWindow();


}

void Gameoflife::Simulation_Automaton2D(){
    Simulation = createNewWindow();
}

void Gameoflife::Simulation_HexaAutomaton(){
    Simulation=createNewWindow();

}

QWidget* Gameoflife::createNewWindow(){
    Simulation = new QWidget();
    Simulation->show();
    this->hide();

    retour=new QPushButton("Retour au menu principal");
    runSimulation = new QPushButton("Lancer la simulation");
    next=new QPushButton("Calculer l'état suivant");

    colone1=new QVBoxLayout();
    colone1->addWidget(runSimulation);
    colone1->addWidget(next);
    colone1->addWidget(retour);

    Simulation->setLayout(colone1);
    connect(retour, SIGNAL(clicked()),this, SLOT(retour_menu()) );
  //  connect(runSimulation, SIGNAL(clicked()), this, SLOT(run());
   // connect(next, SIGNAL(clicked()), this, SLOT(next()));

    return Simulation;
}

void Gameoflife::retour_menu(){
    this->show();
    Simulation->hide();
}
