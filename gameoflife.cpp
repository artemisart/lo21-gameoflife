#include <QString>

#include "Automaton.h"
#include "Grid.h"
#include "History.h"
#include "Rule.h"
#include "gameoflife.h"

Gameoflife::Gameoflife(QWidget* parent)
	: QWidget(parent)
{
    this->setFixedSize(600, 100);

	introduction = new QLabel("Bienvenue dans notre simulateur d'automate, choisissez l'automate que vous souhaitez simuler : ");

	Automaton1D = new QPushButton("Automate 1D");
	Automaton2D = new QPushButton("Automate 2D");
	HexaAutomaton = new QPushButton("Automate Hexadécimal");

	connect(Automaton1D, SIGNAL(clicked()), this, SLOT(Simulation_Automaton1D()));
	connect(Automaton2D, SIGNAL(clicked()), this, SLOT(Simulation_Automaton2D()));
	connect(HexaAutomaton, SIGNAL(clicked()), this, SLOT(Simulation_HexaAutomaton()));

	presentation = new QHBoxLayout();
	presentation->addWidget(Automaton1D);
	presentation->addWidget(Automaton2D);
	presentation->addWidget(HexaAutomaton);

	presentation2 = new QVBoxLayout();
	presentation2->addWidget(introduction);
	presentation2->addLayout(presentation);

    setLayout(presentation2);
}

void Gameoflife::Simulation_Automaton1D()
{
	Simulation = createNewWindow();
    QLabel* sizel = new QLabel("Taille de l'automate");
    sizel->setFixedWidth(200);
    size = new QPushButton("Créer un automate de cette taille");
    size->setFixedSize(200, 25);
	colField = new QLineEdit();
    colField->setFixedSize(50,25);
	colField->setText("20");

	QString dimensionCol = colField->text();
	bool ok = false;
	int dim = dimensionCol.toInt(&ok);

    param = new QHBoxLayout();

    param->addWidget(sizel);
	param->addWidget(colField);
	param->addWidget(size);
    param->addLayout(colone2);

	int taille = 25;

	depart = new QTableWidget(1, dim, Simulation);
	depart->setFixedSize(dim * taille, taille);
	depart->horizontalHeader()->setVisible(false);
	depart->verticalHeader()->setVisible(false);
	depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	for (int i = 0; i < dim; i++) {
		depart->setColumnWidth(i, taille);
		depart->setItem(0, i, new QTableWidgetItem(""));
	}

	grid = new QVBoxLayout();
	grid->addLayout(param);


	connect(size, SIGNAL(clicked()), this, SLOT(setGrid1D()));

    numl=new QLabel("Numero");
    numeroBitl[0] = new QLabel("111");
    numeroBitl[1] = new QLabel("110");
    numeroBitl[2] = new QLabel("101");
    numeroBitl[3] = new QLabel("100");
    numeroBitl[4] = new QLabel("011");
    numeroBitl[5] = new QLabel("010");
    numeroBitl[6]= new QLabel("001");
    numeroBitl[7] = new QLabel("000");

    num = new QSpinBox(Simulation);
    num->setFixedSize(50,25);
    num->setRange(0,255);
    num->setValue(0);

    zeroOneValidator = new QIntValidator(this);
    zeroOneValidator->setRange(0,1);

    numc = new QVBoxLayout;
    numc->addWidget(numl);
    numc->addWidget(num);


    numeroc = new QHBoxLayout;
    numeroc->addLayout(numc);

    for(unsigned int i=0; i<8; i++){
        numeroBit[i] = new QLineEdit(this);
        numeroBit[i]->setFixedWidth(20);
        numeroBit[i]->setMaxLength(1);
        numeroBit[i]->setText("0");
        numeroBit[i]->setValidator(zeroOneValidator);
        bitc[i] = new QVBoxLayout;
        bitc[i]->addWidget(numeroBitl[i]);
        bitc[i]->addWidget(numeroBit[i]);
        numeroc->addLayout(bitc[i]);
    }

    connect(num, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));

    for(unsigned int i=0; i<8; i++){
        connect(numeroBit[i], SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    }

    grid->addLayout(numeroc);
    grid->addWidget(depart);
    colone1->addLayout(grid);
    Simulation->setLayout(wrapper);

}

void Gameoflife::Simulation_Automaton2D()
{
	Simulation = createNewWindow();

	size = new QPushButton("Créer un automate de cette taille");
	colField = new QLineEdit(Simulation);
	rowField = new QLineEdit(Simulation);
	colField->setText("20");
	rowField->setText("20");

	QString dimensionCol = colField->text();
	QString dimensionRow = rowField->text();

	int dimCol = dimensionCol.toInt();
	int dimRow = dimensionRow.toInt();

	param = new QHBoxLayout();

	param->addWidget(colField);
	param->addWidget(rowField);

	param->addWidget(size);
    colone2->addLayout(param);

	int taille = 25;

	depart = new QTableWidget(dimRow, dimCol, Simulation);
	depart->setFixedSize(dimCol * taille, dimRow * taille);
	depart->horizontalHeader()->setVisible(false);
	depart->verticalHeader()->setVisible(false);
	depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	for (int i = 0; i < dimCol; i++) {
		for (int j = 0; j < dimRow; j++) {
			depart->setColumnWidth(i, taille);
			depart->setRowHeight(j, taille);
			depart->setItem(j, i, new QTableWidgetItem(""));
		}
	}

	grid = new QVBoxLayout();
	grid->addLayout(param);
	grid->addWidget(depart);
	Simulation->setLayout(grid);

	connect(size, SIGNAL(clicked()), this, SLOT(setGrid2D()));
}

void Gameoflife::Simulation_HexaAutomaton()
{
	Simulation = createNewWindow();
}

QWidget* Gameoflife::createNewWindow()
{
	Simulation = new QWidget();
    Simulation->setFixedSize(900, 200);

	Simulation->show();
	this->hide();

	retour = new QPushButton("Retour au menu principal");
	runSimulation = new QPushButton("Lancer la simulation");
	next = new QPushButton("Calculer l'état suivant");

    wrapper = new QHBoxLayout();
    colone1=new QVBoxLayout();
    colone2 = new QVBoxLayout();
    colone2->addWidget(runSimulation);
    colone2->addWidget(next);
    colone2->addWidget(retour);
    wrapper->addLayout(colone1);
    wrapper->addLayout(colone2);

	connect(retour, SIGNAL(clicked()), this, SLOT(retour_menu()));
	// connect(runSimulation, SIGNAL(clicked()), this, SLOT(run());
	// connect(next, SIGNAL(clicked()), this, SLOT(next()));

	return Simulation;
}

void Gameoflife::retour_menu()
{
	this->show();
	Simulation->hide();
}

void Gameoflife::setGrid1D()
{
	QString dimensionCol = colField->text();
	bool ok = false;

	int dimCol = dimensionCol.toInt(&ok);

	depart->setColumnCount(dimCol);

	int taille = 25;
	depart->setFixedSize(dimCol * taille, taille);
	depart->horizontalHeader()->setVisible(false);
	depart->verticalHeader()->setVisible(false);
	depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	for (int i = 0; i < dimCol; i++) {
		depart->setColumnWidth(i, taille);
		depart->setItem(0, i, new QTableWidgetItem(""));
	}





}

void Gameoflife::setGrid2D()
{
	QString dimensionCol = colField->text();
	QString dimensionRow = rowField->text();

	int dimCol = dimensionCol.toInt();
	int dimRow = dimensionRow.toInt();

	depart->setColumnCount(dimCol);
	depart->setRowCount(dimRow);

	int taille = 25;

	depart->setFixedSize(dimCol * taille, dimRow * taille);
	depart->horizontalHeader()->setVisible(false);
	depart->verticalHeader()->setVisible(false);
	depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	for (int i = 0; i < dimCol; i++) {
		for (int j = 0; j < dimRow; j++) {
			depart->setColumnWidth(i, taille);
			depart->setRowHeight(j, taille);
			depart->setItem(j, i, new QTableWidgetItem(""));
		}
	}
}



short unsigned int NumBitToNum(const std::string& num) {
    if (num.size() != 8) throw("Numero d'automate indefini");
    int puissance = 1;
    short unsigned int numero = 0;
    for (int i = 7; i >= 0; i--) {
        if (num[i] == '1') numero += puissance;
        else if (num[i] != '0') throw("Numero d'automate indefini");
        puissance *= 2;
    }
    return numero;
}

std::string NumToNumBit(short unsigned int num) {
    std::string numeroBit;
    if (num > 256) throw("Numero d'automate indefini");
    unsigned short int p = 128;
    int i = 7;
    while (i >= 0) {
        if (num >= p) {
            numeroBit.push_back('1');
            num -= p;
        }
        else { numeroBit.push_back('0'); }
        i--;
        p = p / 2;
    }
    return numeroBit;
}


void Gameoflife::synchronizeNumToNumBit(int j){
    std::string numbit=NumToNumBit(j);
    for(unsigned int i=0; i<8; i++){
        numeroBit[i]->setText(QString(numbit[i]));
    }
}

void Gameoflife::synchronizeNumBitToNum(const QString& s){
    for(unsigned int i=0; i<8; i++)
            if (numeroBit[i]->text()=="") return;
    std::string str;
    for(unsigned int i=0; i<8; i++){
        str+=numeroBit[i]->text().toStdString();
    }
    int i=NumBitToNum(str);
    num->setValue(i);

}

