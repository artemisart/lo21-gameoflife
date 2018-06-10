#include "automate_2d.h"
#include "automate_1d.h"
#include "ui_automate_2d.h"
#include <QMessageBox>
#include <unistd.h> //for using the function sleep

Automate_2D::Automate_2D(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::Automate_2D)
    , rang(0)
    , sim(false)
{
    ui->setupUi(this);

	QLineEdit* _survive[9] = {
        ui->Survive0, ui->Survive1, ui->Survive2, ui->Survive3,
        ui->Survive4, ui->Survive5, ui->Survive6, ui->Survive7, ui->Survive8
    };
	QLineEdit* _born[9] = {
        ui->Born0, ui->Born1, ui->Born2, ui->Born3,
		ui->Born4, ui->Born5, ui->Born6, ui->Born7, ui->Born8
    };
	std::copy_n(_survive, 9, survive); // because arrays are not directly assignable
	std::copy_n(_born, 9, born);

	ui->grid->setFixedSize(ui->largeur->value() * 25, ui->hauteur->value() * 25);
    for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
            ui->grid->setColumnWidth(i, 25);
			ui->grid->setRowHeight(j, 25);
			ui->grid->setItem(i, j, new QTableWidgetItem(""));
        }
    }
    h = new RingHistory<Grid<bool, Index2D>>(10);
    r = new Rule2D();
    a = new Automaton<bool, Index2D>(h, r);

	start = new Grid2D<bool>(20, 20);
    h->setStart(*start);

    zeroOneValidator = new QIntValidator(0, 1, this);
	for (int i = 0; i < 9; i++) {
        born[i]->setValidator(zeroOneValidator);
		survive[i]->setValidator(zeroOneValidator);
    }
	/*

    for (auto brn : born) {
        brn->setValidator(zeroOneValidator);
        connect(brn, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum_b(QString)));
    }*/

	/*   for (auto surv : survive) {
        surv->setValidator(zeroOneValidator);
        connect(surv, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum_s(QString)));
    }
*/
    connect(ui->size, SIGNAL(clicked()), this, SLOT(setSize()));
    connect(ui->born, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit_b(int)));
    connect(ui->Survive, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit_s(int)));

    connect(ui->run, SIGNAL(clicked()), this, SLOT(simulation()));

	connect(ui->grid, SIGNAL(clicked(QModelIndex)), this, SLOT(cellActivation(const QModelIndex&)));

    connect(ui->next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->menu, SIGNAL(clicked()), this, SLOT(menu()));
}

Automate_2D::~Automate_2D()
{
    delete ui;
}

void Automate_2D::setSize()
{

	int dimCol = ui->hauteur->value();
    int dimRow = ui->largeur->value();
	ui->grid->setMinimumSize(25 * dimRow, 25 * dimCol);

    ui->grid->setColumnCount(dimCol);
    ui->grid->setRowCount(dimRow);

    ui->grid->horizontalHeader()->setVisible(false);
    ui->grid->verticalHeader()->setVisible(false);
    ui->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (int i = 0; i < dimCol; i++) {
		for (int j = 0; j < dimRow; j++) {
            ui->grid->setColumnWidth(j, 25);
            ui->grid->setRowHeight(i, 25);
			ui->grid->setItem(i, j, new QTableWidgetItem(""));
        }
    }
    auto* g1 = new Grid2D<bool>(dimCol, dimRow);
    h->setStart(*g1);
}

void Automate_2D::synchronizeNumToNumBit_s(int j)
{

    QString numbit = NumToNumBit(j);

    ui->Survive0->setText(QString(numbit[0]));
    ui->Survive1->setText(QString(numbit[1]));
    ui->Survive2->setText(QString(numbit[2]));
    ui->Survive3->setText(QString(numbit[3]));
    ui->Survive4->setText(QString(numbit[4]));
    ui->Survive5->setText(QString(numbit[5]));
    ui->Survive6->setText(QString(numbit[6]));
    ui->Survive7->setText(QString(numbit[7]));

    r->setSurvive(j);
}

void Automate_2D::synchronizeNumToNumBit_b(int j)
{

    QString numbit = NumToNumBit(j);

    ui->Born0->setText(QString(numbit[0]));
    ui->Born1->setText(QString(numbit[1]));
    ui->Born2->setText(QString(numbit[2]));
    ui->Born3->setText(QString(numbit[3]));
    ui->Born4->setText(QString(numbit[4]));
    ui->Born5->setText(QString(numbit[5]));
    ui->Born6->setText(QString(numbit[6]));
    ui->Born7->setText(QString(numbit[7]));

    r->setSurvive(j);
}

void Automate_2D::synchronizeNumBitToNum_s(const QString& s)
{

    QString str;

	for (unsigned int k = 0; k < 8; k++) {
		if (survive[k]->text() == "")
            return;
	}

	for (unsigned int j = 0; j < 8; j++) {
        str += survive[j]->text();
    }

	int i = NumBitToNum(str);
    ui->Survive->setValue(i);
    r->setSurvive(i);
}

void Automate_2D::synchronizeNumBitToNum_b(const QString& s)
{

    QString str;

	for (unsigned int k = 0; k < 8; k++) {
		if (born[k]->text() == "")
            return;
    }

	for (unsigned int j = 0; j < 8; j++) {
        str += born[j]->text();
    }

    int i = NumBitToNum(str);
    ui->born->setValue(i);
    r->setBorn(i);
}

void Automate_2D::simulation()
{
	while (1) {

        sleep(3);
        next();
    }
}

void Automate_2D::cellActivation(const QModelIndex& index)
{
	int col = index.column();
	int row = index.row();
	if (ui->grid->item(row, col)->text() == "") {
		ui->grid->item(row, col)->setText("_");
		ui->grid->item(index.row(), index.column())->setBackgroundColor("black");
		ui->grid->item(row, col)->setTextColor("black");
		start->setCell(Index2D(index.row(), index.column()), true);

	} else {
		ui->grid->item(index.row(), index.column())->setText("");
		ui->grid->item(index.row(), index.column())->setBackgroundColor("white");
		ui->grid->item(index.row(), index.column())->setTextColor("white");
		start->setCell(Index2D(index.row(), index.column()), false);
    }
}

void Automate_2D::next()
{

    //if(rang<ui->nb_etats->value()){
	ui->grid->setEditTriggers(QAbstractItemView::NoEditTriggers);

	a->next();
	auto* grid = h->getLast();

	for (int i = 0; i < ui->largeur->value(); i++) {
		for (int j = 0; j < ui->hauteur->value(); j++) {
			ui->grid->setItem(j, i, new QTableWidgetItem(""));
			if (grid->getCell(Index2D(j, i)) == 1) {
                ui->grid->item(j, i)->setBackgroundColor("black");
			} else {

				ui->grid->item(j, i)->setBackgroundColor("white");
            }
        }
	}

	incRang();
	/* }
    else {
        QMessageBox::warning(
            this,
            tr("Game Of Life"),
            tr("You have already reached the maximum number of states in the simulation") );
    }*/
}

void Automate_2D::menu()
{
    this->hide();
    this->parent->show();
}
/*
void Automate_2D::init_simulation(){
    etats = new QTableWidget(ui->hauteur->value(), ui->largeur->value(), this);
    etats->setFixedSize(ui->hauteur->value()*25, ui->largeur->value() * 25);
    etats->horizontalHeader()->setVisible(false);
    etats->verticalHeader()->setVisible(false);
    etats->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    etats->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (int i = 0; i < 20; i++) {
        for(unsigned int j=0; j<20; j++){
            etats->setColumnWidth(i, 25);
            etats->setRowHeight(j,25);
            etats->setItem( i,j, new QTableWidgetItem(""));
        }

    }
    //non editable
    etats->setEditTriggers(QAbstractItemView::NoEditTriggers);

    incRang();

    ui->simulation->addWidget(etats);


}

*/
