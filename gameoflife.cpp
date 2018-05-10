#include"Automaton.h"
#include"gameoflife.h"
#include"Rule.h"
#include"Grid.h"
#include"History.h"
#include<QString>

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
  size = new QPushButton("Créer un automate de cette taille");
  colField=new QLineEdit();
  colField->setText("20");

  QString dimensionCol=colField->text();

  bool ok = false;

  int dim=dimensionCol.toInt(&ok);


  param = new QHBoxLayout();

  param->addWidget(colField);
  param->addWidget(size);
  param->addLayout(colone1);

  unsigned int taille = 25;

  depart=new QTableWidget(1,dim, Simulation);
  depart->setFixedSize(dim*taille, taille);
  depart->horizontalHeader()->setVisible(false);
  depart->verticalHeader()->setVisible(false);
  depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  for(unsigned int i=0; i<dim; i++){
      depart->setColumnWidth(i, taille);
      depart->setItem(0, i, new QTableWidgetItem(""));
  }

  grid=new QVBoxLayout();
  grid->addLayout(param);
  grid->addWidget(depart);
  Simulation->setLayout(grid);


  connect(size, SIGNAL(clicked()), this, SLOT(setGrid1D()));




}

void Gameoflife::Simulation_Automaton2D(){
    Simulation = createNewWindow();

    size = new QPushButton("Créer un automate de cette taille");
    colField = new QLineEdit(Simulation);
    rowField = new QLineEdit(Simulation);
    colField->setText("20");
    rowField->setText("20");

    QString dimensionCol=colField->text();
    QString dimensionRow=rowField->text();


    int dimCol=dimensionCol.toInt();
    int dimRow=dimensionRow.toInt();


    param = new QHBoxLayout();

    param->addWidget(colField);
    param->addWidget(rowField);

    param->addWidget(size);
    param->addLayout(colone1);

    unsigned int taille = 25;

    depart=new QTableWidget(dimRow,dimCol, Simulation);
    depart->setFixedSize(dimCol*taille, dimRow*taille);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    for(unsigned int i=0; i<dimCol; i++){
        for(unsigned int j=0; j<dimRow; j++){
            depart->setColumnWidth(i, taille);
            depart->setRowHeight(j, taille);
            depart->setItem(j, i, new QTableWidgetItem(""));
        }
    }

    grid=new QVBoxLayout();
    grid->addLayout(param);
    grid->addWidget(depart);
    Simulation->setLayout(grid);


    connect(size, SIGNAL(clicked()), this, SLOT(setGrid2D()));
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

    connect(retour, SIGNAL(clicked()),this, SLOT(retour_menu()) );
  //  connect(runSimulation, SIGNAL(clicked()), this, SLOT(run());
   // connect(next, SIGNAL(clicked()), this, SLOT(next()));

    return Simulation;
}

void Gameoflife::retour_menu(){
    this->show();
    Simulation->hide();
}


void Gameoflife::setGrid1D(){
    QString dimensionCol=colField->text();
    bool ok = false;

    int dimCol=dimensionCol.toInt(&ok);

    depart->setColumnCount(dimCol);

    unsigned int taille = 25;
    depart->setFixedSize(dimCol*taille, taille);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    for(unsigned int i=0; i<dimCol; i++){
        depart->setColumnWidth(i, taille);
        depart->setItem(0, i, new QTableWidgetItem(""));
    }

}

void Gameoflife::setGrid2D(){
    QString dimensionCol=colField->text();
    QString dimensionRow=rowField->text();


    int dimCol=dimensionCol.toInt();
    int dimRow=dimensionRow.toInt();

    depart->setColumnCount(dimCol);
    depart->setRowCount(dimRow);

    unsigned int taille = 25;

    depart->setFixedSize(dimCol*taille, dimRow*taille);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    for(unsigned int i=0; i<dimCol; i++){
        for(unsigned int j=0; j<dimRow; j++){
            depart->setColumnWidth(i, taille);
            depart->setRowHeight(j, taille);
            depart->setItem(j, i, new QTableWidgetItem(""));
        }
    }


}
