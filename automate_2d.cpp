#include "automate_2d.h"
#include "automate_1d.h"
#include "ui_automate_2d.h"
#include <QMessageBox>

Automate_2D::Automate_2D(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::Automate_2D)
    , rang(0)
    , sim(false)
{
    ui->setupUi(this);
    QLineEdit* _numBits[8] = {
        ui->numBit1, ui->numBit2, ui->numBit3, ui->numBit4,
        ui->numBit5, ui->numBit6, ui->numBit7, ui->numBit8
    };
    std::copy_n(_numBits, 8, numBits); // because numBits is not directly assignable

    ui->grid->setFixedSize(ui->hauteur->value()* 25, ui->largeur->value()* 25);
    for (int i = 0; i < 20; i++) {
        for(unsigned int j=0; j<20; j++){
            ui->grid->setColumnWidth(i, 25);
            ui->grid->setRowHeight(j,25);
            ui->grid->setItem( i,j, new QTableWidgetItem(""));
        }

    }
    h = new RingHistory<Grid<bool, Index2D>>(10);
    r = new Rule2D();
    a = new Automaton<bool, Index2D>(h, r);

    start = new Grid2D<bool>(20,20);
    h->setStart(*start);

    zeroOneValidator = new QIntValidator(0, 1, this);
    for (auto nb : numBits) {
        nb->setValidator(zeroOneValidator);
        connect(nb, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    }

    connect(ui->size, SIGNAL(clicked()), this, SLOT(setSize()));
    connect(ui->rule, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));

    connect(ui->run, SIGNAL(clicked()), this, SLOT(simulation()));

    connect(ui->grid, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(cellActivation(const QModelIndex&)));

    connect(ui->next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->menu, SIGNAL(clicked()), this, SLOT(menu()));
}

Automate_2D::~Automate_2D()
{
    delete ui;
}

void Automate_2D::setSize()
{


    int dimCol =ui->hauteur->value();
    int dimRow = ui->largeur->value();

    ui->grid->setColumnCount(dimCol);
    ui->grid->setRowCount(dimRow);

    ui->grid->horizontalHeader()->setVisible(false);
    ui->grid->verticalHeader()->setVisible(false);
    ui->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    for (int i = 0; i < dimCol; i++) {
		for (int j = 0; j < dimRow; j++) {
			ui->grid->setColumnWidth(i, 25);
            ui->grid->setRowHeight(j, 25);
            ui->grid->setItem( i,j, new QTableWidgetItem(""));
        }
    }
    auto* g1 = new Grid2D<bool>(dimCol, dimRow);
    h->setStart(*g1);


}

void Automate_2D::synchronizeNumToNumBit(int j)
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

    r->setBorn(j);

}

void Automate_2D::synchronizeNumBitToNum(const QString& s)
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
    r->setBorn(i);

}

void Automate_2D::simulation()
{
    while(getRang()<ui->nb_etats->value()){next();}

}

void Automate_2D::cellActivation(const QModelIndex& index)
{
	int col = index.column();
	int row = index.row();
	if (ui->grid->item(row, col)->text() == "") {
		ui->grid->item(row, col)->setText("_");
		ui->grid->item(index.row(), index.column())->setBackgroundColor("black");
		ui->grid->item(row, col)->setTextColor("black");
        start->setCell(Index2D(index.row(),index.column()), true);


	} else {
		ui->grid->item(index.row(), index.column())->setText("");
		ui->grid->item(index.row(), index.column())->setBackgroundColor("white");
		ui->grid->item(index.row(), index.column())->setTextColor("white");
        start->setCell(Index2D(index.row(),index.column()), false);

	}
}

void Automate_2D::next()
{

    if(rang<ui->nb_etats->value()){

        a->next();
       /* int s=getRang();
        auto* grid = h->getLast();

        init_simulation();

        for(int i=0; i< ui->largeur->value(); i++){
            for (int j = 0; j < ui->hauteur->value(); j++) {
                etats[s]->setItem(j, i, new QTableWidgetItem(""));
                if(grid->getCell(Index2D(j, i))==1){
                etats[s]->item(j, i)->setBackgroundColor("black");
                }
                else{

                  etats[s]->item(j, i)->setBackgroundColor("white");
                }
            }


        }


        incRang();*/
    }
    else {
        QMessageBox::warning(
            this,
            tr("Game Of Life"),
            tr("You have already reached the maximum number of states in the simulation") );
    }

}

void Automate_2D::menu()
{
    this->hide();
    this->parent->show();
}

void Automate_2D::init_simulation(){
    int a = getRang();
    etats[a] = new QTableWidget(ui->hauteur->value(), ui->largeur->value(), this);
    etats[a]->setFixedSize(ui->hauteur->value() * 25, ui->largeur->value() * 25);
    etats[a]->horizontalHeader()->setVisible(false);
    etats[a]->verticalHeader()->setVisible(false);
    etats[a]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    etats[a]->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //non editable
    etats[a]->setEditTriggers(QAbstractItemView::NoEditTriggers);

    incRang();

    ui->simulation->addWidget(etats[a]);

}
