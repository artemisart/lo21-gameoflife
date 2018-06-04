#include "automate_2d.h"
#include "ui_automate_2d.h"
#include "automate_1d.h"

Automate_2D::Automate_2D(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Automate_2D)
{
    ui->setupUi(this);

    for (int i = 0; i < 20; i++) {
        for(int j=0; j<20; j++) {

            ui->grid->setColumnWidth(i,25);
            ui->grid->setRowHeight(j, 25);
            ui->grid->setItem(j, i, new QTableWidgetItem(""));
        }
      }

    zeroOneValidator = new QIntValidator(this);
    zeroOneValidator->setRange(0,1);

    ui->numBit1->setValidator(zeroOneValidator);
    ui->numBit2->setValidator(zeroOneValidator);
    ui->numBit3->setValidator(zeroOneValidator);
    ui->numBit4->setValidator(zeroOneValidator);
    ui->numBit5->setValidator(zeroOneValidator);
    ui->numBit6->setValidator(zeroOneValidator);
    ui->numBit7->setValidator(zeroOneValidator);
    ui->numBit8->setValidator(zeroOneValidator);



    connect(ui->size, SIGNAL(clicked()), this, SLOT(setSize()));

    connect(ui->rule, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));


    connect(ui->numBit1, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    connect(ui->numBit2, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    connect(ui->numBit3, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    connect(ui->numBit4, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    connect(ui->numBit5, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    connect(ui->numBit6, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    connect(ui->numBit7, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    connect(ui->numBit8, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));


    connect(ui->run, SIGNAL(clicked()), this, SLOT(simulation()));

    connect(ui->grid, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(cellActivation(const QModelIndex&)));

    connect(ui->next, SIGNAL(clicked()), this, SLOT(next()));

    connect(ui->menu, SIGNAL(clicked()), this, SLOT(menu()));
}

Automate_2D::~Automate_2D()
{
    delete ui;
}


void Automate_2D::setSize(){

    ui->grid->clearSelection();

    QString dimensionCol = ui->hauteur->text();
    QString dimensionRow = ui->largeur->text();

    int dimCol = dimensionCol.toInt();
    int dimRow = dimensionRow.toInt();

    ui->grid->setColumnCount(dimCol);
    ui->grid->setRowCount(dimRow);

    for (int i = 0; i < dimCol; i++) {
        for(int j=0; j<dimRow; j++){
            ui->grid->setColumnWidth(i,25);
            ui->grid->setRowHeight(j, 25);
            ui->grid->setItem(j, i, new QTableWidgetItem(""));

        }

    }
}




void Automate_2D::synchronizeNumToNumBit(int j){
    ui->grid->clearSelection();

     QString numbit=NumToNumBit(j);
     ui->numBit1->setText(QString(numbit[0]));
     ui->numBit2->setText(QString(numbit[1]));
     ui->numBit3->setText(QString(numbit[2]));
     ui->numBit4->setText(QString(numbit[3]));
     ui->numBit5->setText(QString(numbit[4]));
     ui->numBit6->setText(QString(numbit[5]));
     ui->numBit7->setText(QString(numbit[6]));
     ui->numBit8->setText(QString(numbit[7]));

}

void Automate_2D::synchronizeNumBitToNum(const QString& s){


    QString str;

    if(ui->numBit1->text()=="" || ui->numBit2->text()=="" ||ui->numBit3->text()=="" || ui->numBit4->text()=="" || ui->numBit5->text()=="" || ui->numBit6->text()=="" || ui->numBit7->text()=="" || ui->numBit8->text()=="" ) return;

    str+=ui->numBit1->text();
    str+=ui->numBit2->text();
    str+=ui->numBit3->text();
    str+=ui->numBit4->text();
    str+=ui->numBit5->text();
    str+=ui->numBit6->text();
    str+=ui->numBit7->text();
    str+=ui->numBit8->text();


    int i=NumBitToNum(str);
    ui->rule->setValue(i);

}

void Automate_2D::simulation(){

        }

void Automate_2D::cellActivation(const QModelIndex& index){
    int col=index.column();
    int row=index.row();
       if(ui->grid->item(row, col)->text()==""){
          ui->grid->item(row, col)->setText("_");
          ui->grid->item(index.row(), index.column())->setBackgroundColor("black");
          ui->grid->item(row, col)->setTextColor("black");

        }else {
           ui->grid->item(index.row(), index.column())->setText("");
           ui->grid->item(index.row(), index.column())->setBackgroundColor("white");
           ui->grid->item(index.row(), index.column())->setTextColor("white");

            }
}

void Automate_2D::next(){

}


void Automate_2D::menu(){
    this->hide();
    this->parent->show();
}

