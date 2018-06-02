#include "automate_1d.h"
#include "ui_automate_1d.h"
#include<iostream>
#include"Rule.h"

Automate_1D::Automate_1D(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Automate_1D)
{
    ui->setupUi(this);


    for (int i = 0; i < 20; i++) {
        ui->grid->setColumnWidth(i,25);
        ui->grid->setItem(0, i, new QTableWidgetItem(""));    }

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

Automate_1D::~Automate_1D()
{
    delete ui;
}

void Automate_1D::setSize(){

    QString dimensionCol = ui->size_Box->text();

    int dimCol = dimensionCol.toInt();

    ui->grid->setColumnCount(dimCol);

    ui->grid->horizontalHeader()->setVisible(false);
        ui->grid->verticalHeader()->setVisible(false);
        ui->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for (int i = 0; i < dimCol; i++) {
        ui->grid->setColumnWidth(i,25);
        ui->grid->setItem(0, i, new QTableWidgetItem(""));

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


void Automate_1D::synchronizeNumToNumBit(int j){
    std::string numbit=NumToNumBit(j);
     ui->numBit1->setText(QString(numbit[0]));
     ui->numBit2->setText(QString(numbit[1]));
     ui->numBit3->setText(QString(numbit[2]));
     ui->numBit4->setText(QString(numbit[3]));
     ui->numBit5->setText(QString(numbit[4]));
     ui->numBit6->setText(QString(numbit[5]));
     ui->numBit7->setText(QString(numbit[6]));
     ui->numBit8->setText(QString(numbit[7]));

}

void Automate_1D::synchronizeNumBitToNum(const QString& s){
    std::string str;
    str+=ui->numBit1->text().toStdString();
    str+=ui->numBit2->text().toStdString();
    str+=ui->numBit3->text().toStdString();
    str+=ui->numBit4->text().toStdString();
    str+=ui->numBit5->text().toStdString();
    str+=ui->numBit6->text().toStdString();
    str+=ui->numBit7->text().toStdString();
    str+=ui->numBit8->text().toStdString();


    int i=NumBitToNum(str);
    ui->rule->setValue(i);

}

void Automate_1D::simulation(){

        }

void Automate_1D::cellActivation(const QModelIndex& index){
       if(ui->grid->item(0, index.column())->text()==""){

           ui->grid->item(0, index.column())->setText("_");
           ui->grid->item(0, index.column())->setBackgroundColor("black");
           ui->grid->item(0, index.column())->setTextColor("black");

        }else {
           ui->grid->item(0, index.column())->setText("");
           ui->grid->item(0, index.column())->setBackgroundColor("white");
           ui->grid->item(0, index.column())->setTextColor("white");

            }
}

void Automate_1D::next(){

}


void Automate_1D::menu(){
    this->hide();
    this->parent->show();
}
