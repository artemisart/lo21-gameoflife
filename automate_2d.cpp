#include "automate_2d.h"
#include "automate_1d.h"
#include "ui_automate_2d.h"
#include <QMessageBox>
#include<QScrollArea>
#include <unistd.h>     //for using the function sleep
#include <QTimer>
#include <QFileDialog>

Automate_2D::Automate_2D(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::Automate_2D)
    , rang(0)
    , sim(true)
{
    ui->setupUi(this);

    timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));

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
    //if(this->getType() == 0)
    h = new RingHistory<Grid<bool, Index2D>>(10);
   // if(this->getType()== 1) h = new RingHistory<Grid<uint8_t, Index2D> >(10);
    r = new Rule2D();
    a = new Automaton<bool, Index2D>(h, r);

	start = new Grid2D<bool>(20, 20);
    h->setStart(*start);

    zeroOneValidator = new QIntValidator(0, 1, this);
	for (int i = 0; i < 9; i++) {
        born[i]->setValidator(zeroOneValidator);
		survive[i]->setValidator(zeroOneValidator);
    }


    for (auto brn : born) {
        brn->setValidator(zeroOneValidator);
        connect(brn, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum_b(QString)));
    }

   for (auto surv : survive) {
        surv->setValidator(zeroOneValidator);
        connect(surv, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum_s(QString)));
    }

    connect(ui->size, SIGNAL(clicked()), this, SLOT(setSize()));
    connect(ui->born, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit_b(int)));
    connect(ui->Survive, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit_s(int)));

    connect(ui->run, SIGNAL(clicked()), this, SLOT(simulation()));

	connect(ui->grid, SIGNAL(clicked(QModelIndex)), this, SLOT(cellActivation(const QModelIndex&)));

    connect(ui->next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->menu, SIGNAL(clicked()), this, SLOT(menu()));
    connect(ui->reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->stop, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(ui->load, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->random, SIGNAL(clicked(bool)), this, SLOT(rand()));
    connect(ui->random_sym, SIGNAL(clicked(bool)), this, SLOT(rand_sym()));

}

Automate_2D::~Automate_2D()
{
    delete ui;
}

void Automate_2D::reset(){
    ui->largeur->setValue(20);
    ui->hauteur->setValue(0);
    ui->Survive->setValue(0);
    ui->born->setValue(0);
    setSize();

    for(int i=0; i<20;i++){
        for(int j=0; j<20; j++){
            start->setCell(Index2D(i,j), true);
            ui->grid->item(i,j)->setText("");


        }
    }
    ui->grid->setEditTriggers(QAbstractItemView::DoubleClicked);


}

void Automate_2D::stop(){
    sim = false;
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

    sim=true;
    run();





}

void Automate_2D::run(){
    if(sim==true){
        timer->start(ui->timer->value()*1000);

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

}

void Automate_2D::menu()
{
    this->hide();
    this->parent->show();
}

void Automate_2D::save(){
  try{
    QString fileName = QFileDialog::getSaveFileName(this,
           tr("save grid"), "",
           tr("lo21 (*.2Dlo21)"));
    if (fileName.isEmpty())
            return;
    std::string name = fileName.toStdString();
    const Grid<bool, Index2D>* g2D= h->getLast();
    g2D->save(name);
    r->save(name);
    std::cout<< "sauvegarde reussie \n";
  } catch (const std::string& e) {
         std::cout << "erreur: " << e << "\n";
  }
}

void Automate_2D::load(){
  try{
    QString fileName = QFileDialog::getOpenFileName(this,
           tr("load grid"), "",
           tr("lo21 (*.2Dlo21)"));
    if (fileName.isEmpty())
            return;
    std::string name = fileName.toStdString();
    Grid2D<bool>* g2D = new Grid2D<bool>(10,10);
    g2D->load(name);
    h->push(g2D);
    Index2D i = g2D->getSize();
    ui->largeur->setValue(i.col);
    ui->hauteur->setValue(i.row);  //met la taille a jour
    this->setSize();

    for(Index2D i; i.row < g2D->getSize().row; ++i.row){
        for(i.col=0; i.col < g2D->getSize().col; ++i.col){
                bool a=g2D->getCell(i);
                if(a==0){
                    ui->grid->item(i.row,i.col)->setBackgroundColor("white");
                    ui->grid->item(i.row, i.col)->setText("");
                    ui->grid->item(i.row, i.col)->setBackgroundColor("white");
                }
                else {
                    ui->grid->item(i.row,i.col)->setBackgroundColor("black");
                    ui->grid->item(i.row, i.col)->setText("_");
                    ui->grid->item(i.row, i.col)->setBackgroundColor("black");
                }
       }
    }

    r->load(name);
    ui->Survive->setValue(r->getSurvive()); //met les regles a jour
    ui->born->setValue(r->getBorn());
    this->synchronizeNumToNumBit_b(r->getBorn());
    this->synchronizeNumToNumBit_s(r->getSurvive());
    std::cout<< "loading reussi";
  } catch (const std::string& e) {
    std::cout << "erreur: " << e << "\n";
  }
}


void Automate_2D::rand(){
    for(unsigned int j=0; j < ui->hauteur->value() ; j++){
        for(unsigned int i=0; i<ui->largeur->value(); i++){
            int a=std::rand()%2;
            if(a==0){
                ui->grid->item(j,i)->setBackgroundColor("white");
                start->setCell(Index2D(j,i), false);
            }
            else {
                ui->grid->item(j,i)->setBackgroundColor("black");
                start->setCell(Index2D(j,i), true);
            }
        }

    }

}

void Automate_2D::rand_sym(){

    for(unsigned int j=0; j<(ui->hauteur->value())/2; j++){
        for(unsigned int i=0; i<(ui->largeur->value()) ; i++){
            int a=std::rand()%2;
            if(a==0){
                ui->grid->item(j,i)->setBackgroundColor("white");
                start->setCell(Index2D(j,i), false);

            }
            else {
                ui->grid->item(j,i)->setBackgroundColor("black");
                start->setCell(Index2D(j,i), true);
            }

    }



}
    int half= std::ceil((ui->hauteur->value())/2) -1;
    int i=0;
    for(unsigned int j=1; j<= half+1; j++){
        for(unsigned int k=0; k<ui->largeur->value(); k++){

            if(ui->grid->item(half-i, k)->backgroundColor() == "white"){
                ui->grid->item( half+j, k)->setBackgroundColor("white");
                start->setCell(Index2D(half+j,k), false);

            }else{
                ui->grid->item( half+j, k)->setBackgroundColor("black");
                start->setCell(Index2D(half+j,k), true);



            }
        }
            i++;
       }


}
