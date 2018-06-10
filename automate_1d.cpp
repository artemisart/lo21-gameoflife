#include "automate_1d.h"
#include "ui_automate_1d.h"
#include <QMessageBox>

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
	for (int i = 0; i < 8; ++i)
		numBits[i]->setText(QString(numbit[i]));
	r->setNum(j);
}

void Automate_1D::synchronizeNumBitToNum(const QString& s)
{
    QString str;

	for (auto nb : numBits) {
		if (nb->text().size() == 0)
			return;
		str += nb->text();
	}

	int i = NumBitToNum(str);
    ui->rule->setValue(i);
    r->setNum(i);
}

void Automate_1D::simulation()
{
	// init_simulation(ui->nb_etats->value());
	while (getRang() < ui->nb_etats->value())
		next();
}

void Automate_1D::cellActivation(const QModelIndex& index)
{
	int col = index.column();
	bool newVal = !start->getCell(col);
	ui->grid->item(0, col)->setBackgroundColor(newVal ? "black" : "white");
	start->setCell(col, newVal);
}

void Automate_1D::next()
{
    if (sim == false) {
		init_simulation(1);
    }
    if(rang < ui->nb_etats->value()){

        a->next();

        auto* grid = h->getLast();

		etats->setRowCount(etats->rowCount() + 1);

        for (int j = 0; j < ui->size_Box->value(); j++) {
            etats->setItem(getRang(), j, new QTableWidgetItem(""));
			bool val = grid->getCell(j);
			etats->item(getRang(), j)->setBackgroundColor(val ? "black" : "white");
        }

        incRang();
	} else {
        QMessageBox::warning(
            this,
            tr("Game Of Life"),
			tr("You have already reached the maximum number of states in the simulation"));
    }
}

void Automate_1D::menu()
{
    this->hide();
    this->parent->show();
}

void Automate_1D::init_simulation(int row)
{
    etats = new QTableWidget(row, ui->size_Box->value(), this);
	etats->setFixedSize(ui->size_Box->value() * 25, ui->nb_etats->value() * 25);
    etats->horizontalHeader()->setVisible(false);
    etats->verticalHeader()->setVisible(false);
    etats->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    etats->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//non editable
    etats->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 0; i < row; i++) {

		for (int j = 0; j < ui->size_Box->value(); j++) {
            etats->setColumnWidth(j, 25);
			etats->setRowHeight(i, 25);
			etats->setItem(i, j, new QTableWidgetItem(""));
        }
    }

	for (int k = 0; k < ui->size_Box->value(); k++)
		etats->item(0, k)->setBackgroundColor(start->getCell(k) ? "black" : "white");
    incRang();

    ui->interface_2->addWidget(etats);
    ui->interface_2->removeWidget(ui->grid);

	sim = true;
}
