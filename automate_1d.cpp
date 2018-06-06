#include "automate_1d.h"
#include "ui_automate_1d.h"

Automate_1D::Automate_1D(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::Automate_1D)
	, rang(0)
	, sim(false)
{
    ui->setupUi(this);
	QLineEdit* _numBits[8] = {
		ui->numBit1, ui->numBit2, ui->numBit3, ui->numBit4,
		ui->numBit5, ui->numBit6, ui->numBit7, ui->numBit8
	};
	std::copy_n(_numBits, 8, numBits); // because numBits is not directly assignable

	ui->grid->setFixedSize(ui->size_Box->value() * 25, 25);
    for (int i = 0; i < 20; i++) {
		ui->grid->setColumnWidth(i, 25);
        ui->grid->setItem(0, i, new QTableWidgetItem(""));
    }

    h = new RingHistory<Grid<bool, Index1D>>(10);
    r = new Rule1D();
    a = new Automaton<bool, Index1D>(h, r);

	start = new Grid1D<bool>(20);
    h->setStart(*start);

	zeroOneValidator = new QIntValidator(0, 1, this);
	for (auto nb : numBits) {
		nb->setValidator(zeroOneValidator);
		connect(nb, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
	}

    connect(ui->size, SIGNAL(clicked()), this, SLOT(setSize()));
    connect(ui->rule, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));

    connect(ui->run, SIGNAL(clicked()), this, SLOT(simulation()));

	connect(ui->grid, SIGNAL(clicked(QModelIndex)), this, SLOT(cellActivation(const QModelIndex&)));

    connect(ui->next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->menu, SIGNAL(clicked()), this, SLOT(menu()));
}

Automate_1D::~Automate_1D()
{
    delete ui;
}

void Automate_1D::setSize()
{
	int dimCol = ui->size_Box->value();
    ui->grid->setColumnCount(dimCol);

    ui->grid->horizontalHeader()->setVisible(false);
	ui->grid->verticalHeader()->setVisible(false);
	ui->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (int i = 0; i < dimCol; i++) {
		ui->grid->setColumnWidth(i, 25);
        ui->grid->setItem(0, i, new QTableWidgetItem(""));
    }
    auto* g1 = new Grid1D<bool>(dimCol);
    //delete h; est-ce necessaire ?
    h->setStart(*g1);
}

short unsigned int NumBitToNum(const QString& num)
{
	if (num.size() != 8)
		// FIXME cette condition va pas du tout, dès qu'on a 00 ou 01 dans un champ ça fait tout crasher
		throw("Numero d'automate indefini");
    int puissance = 1;
    short unsigned int numero = 0;
    for (int i = 7; i >= 0; i--) {
		if (num[i] == '1')
			numero += puissance;
		else if (num[i] != '0')
			throw("Numero d'automate indefini");
        puissance *= 2;
    }
    return numero;
}

QString NumToNumBit(short unsigned int num)
{
    QString numeroBit;
	if (num > 256)
		throw("Numero d'automate indefini");
    unsigned short int p = 128;
    int i = 7;
    while (i >= 0) {
        if (num >= p) {
            numeroBit.push_back('1');
            num -= p;
		} else {
			numeroBit.push_back('0');
		}
        i--;
        p = p / 2;
    }
    return numeroBit;
}

void Automate_1D::synchronizeNumToNumBit(int j)
{
	QString numbit = NumToNumBit(j);
	ui->numBit1->setText(QString(numbit[0]));
	ui->numBit2->setText(QString(numbit[1]));
	ui->numBit3->setText(QString(numbit[2]));
	ui->numBit4->setText(QString(numbit[3]));
	ui->numBit5->setText(QString(numbit[4]));
	ui->numBit6->setText(QString(numbit[5]));
	ui->numBit7->setText(QString(numbit[6]));
	ui->numBit8->setText(QString(numbit[7]));

	r->setNum(j);
}

void Automate_1D::synchronizeNumBitToNum(const QString& s)
{
    QString str;

	if (ui->numBit1->text() == "" || ui->numBit2->text() == "" || ui->numBit3->text() == "" || ui->numBit4->text() == "" || ui->numBit5->text() == "" || ui->numBit6->text() == "" || ui->numBit7->text() == "" || ui->numBit8->text() == "")
		return;

	str += ui->numBit1->text();
	str += ui->numBit2->text();
	str += ui->numBit3->text();
	str += ui->numBit4->text();
	str += ui->numBit5->text();
	str += ui->numBit6->text();
	str += ui->numBit7->text();
	str += ui->numBit8->text();

	int i = NumBitToNum(str);
    ui->rule->setValue(i);
    r->setNum(i);
}

void Automate_1D::simulation()
{
	initSimulation(); /*
    int j;
    int i;
    for(j=0; j< ui->nb_etats->value(); j++)

        for(i=0; i<ui->size_Box->value(); i++){
          if( h->getLast()->getCell(i)){
              ui->grid->item(j,i)->setBackgroundColor("black");

          }
          else{
              ui->grid->item(j,i)->setBackgroundColor("white");
          }
      }
    inc_Rang(); */
}

void Automate_1D::cellActivation(const QModelIndex& index)
{
	if (ui->grid->item(0, index.column())->text() == "") {

		ui->grid->item(0, index.column())->setText("_");
		ui->grid->item(0, index.column())->setBackgroundColor("black");
		ui->grid->item(0, index.column())->setTextColor("black");
		start->setCell(Index1D(index.column()), true);

	} else {
		ui->grid->item(0, index.column())->setText("");
		ui->grid->item(0, index.column())->setBackgroundColor("white");
		ui->grid->item(0, index.column())->setTextColor("white");
		start->setCell(Index1D(index.column()), false);
	}
}

void Automate_1D::next()
{
	if (sim == false) {
		initSimulation();
	} else {
		a->next();

		auto lastRow = h->getLast();
		for (int i = 0; i < ui->size_Box->value(); i++) {
			auto color = lastRow->getCell(i) ? "black" : "white";
			ui->grid->item(getRang(), i)->setBackgroundColor(color);
        }
		incRang();
    }
}

void Automate_1D::menu()
{
    this->hide();
    this->parent->show();
}

void Automate_1D::initSimulation()
{
	etats = new QTableWidget(ui->nb_etats->value(), ui->size_Box->value(), this);
	etats->setFixedSize(ui->size_Box->value() * 25, ui->nb_etats->value() * 25);
    etats->horizontalHeader()->setVisible(false);
    etats->verticalHeader()->setVisible(false);
    etats->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    etats->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//non editable
    etats->setEditTriggers(QAbstractItemView::NoEditTriggers);

	for (int i = 0; i < ui->nb_etats->value(); i++) {

		for (int j = 0; j < ui->size_Box->value(); j++) {
            etats->setColumnWidth(j, 25);
			etats->setRowHeight(i, 25);
			etats->setItem(i, j, new QTableWidgetItem(""));
        }
    }

    ui->interface_2->addWidget(etats);

	sim = true;
}
