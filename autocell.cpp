#include "automate.h"
#include "autocell.h"

unsigned int AutoCell::dimension=25;

AutoCell::AutoCell (QWidget* parent) {

    numl=new QLabel("Numero");
    numeroBitl[0] = new QLabel("111");
    numeroBitl[1] = new QLabel("110");
    numeroBitl[2] = new QLabel("101");
    numeroBitl[3] = new QLabel("100");
    numeroBitl[4] = new QLabel("011");
    numeroBitl[5] = new QLabel("010");
    numeroBitl[6]= new QLabel("001");
    numeroBitl[7] = new QLabel("000");

    num = new QSpinBox(this);
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

    couche=new QVBoxLayout;
    couche->addLayout(numeroc);

    depart = new QTableWidget(1,dimension, this);
    couche->addWidget(depart);
    setLayout(couche);

    unsigned int taille = 25;
    depart->setFixedSize(dimension*taille, taille);
    depart->horizontalHeader()->setVisible(false);
    depart->verticalHeader()->setVisible(false);
    depart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    depart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    for(unsigned int i=0; i<dimension; i++){
        depart->setColumnWidth(i, taille);
        depart->setItem(0, i, new QTableWidgetItem(""));
    }
    connect(depart, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(cellActivation(const QModelIndex&)));
    connect(num, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));

    for(unsigned int i=0; i<8; i++){
        connect(numeroBit[i], SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    }

    simulation=new QPushButton("Simulateur");
    couche->addWidget(simulation);
    connect(simulation, SIGNAL(clicked()),this, SLOT(faireSimulation()) );

    etats=new QTableWidget(dimension, dimension, this);
    etats->setFixedSize(dimension*taille, dimension*taille);
    etats->horizontalHeader()->setVisible(false);
    etats->verticalHeader()->setVisible(false);
    etats->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    etats->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

     //non editable
    etats->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for(unsigned int i=0; i<dimension; i++){
        etats->setColumnWidth(i, taille);
        etats->setRowHeight(i,taille);
        for(unsigned int j=0; j<dimension; j++){
            etats->setItem(j, i, new QTableWidgetItem(""));
        }
    }
   couche ->addWidget(etats);
}

void AutoCell::faireSimulation(){
    Etat e(dimension);
    for(unsigned int i=0; i<dimension; i++){
        if(depart->item(0,i)->text()  != "") e.setCellule(i, true);

    }
   /* for(unsigned int j=0; j<dimension; j++){
        if(e.getCellule(j)){
            etats->item(0,j)->setBackgroundColor("black");

         }else{
             etats->item(0,j)->setBackgroundColor("white");
         }

     }*/

   const Automate& A=
   AutomateManager::getAutomateManager().getAutomate(num->value());

   Simulateur S(A, e);
   for(unsigned int i=0; i<dimension; i++){
       const Etat& d=S.dernier();
       for(unsigned int j=0; j<dimension; j++){
           if(d.getCellule(j)){
               etats->item(i,j)->setBackgroundColor("black");

            }else{
                etats->item(i,j)->setBackgroundColor("white");
            }

        }
     S.next();

    }

}

void AutoCell::cellActivation(const QModelIndex& index){
    if(depart->item(0, index.column())->text()==""){
        //désactivé
        depart->item(0, index.column())->setText("_");
        depart->item(0, index.column())->setBackgroundColor("black");
        depart->item(0, index.column())->setTextColor("black");

    }else {
        depart->item(0, index.column())->setText("");
        depart->item(0, index.column())->setBackgroundColor("white");
        depart->item(0, index.column())->setTextColor("white");

    }
}



void AutoCell::synchronizeNumToNumBit(int j){
    std::string numbit=NumToNumBit(j);
    for(unsigned int i=0; i<8; i++){
        numeroBit[i]->setText(QString(numbit[i]));
    }
}

void AutoCell::synchronizeNumBitToNum(const QString& s){
    for(unsigned int i=0; i<8; i++)
            if (numeroBit[i]->text()=="") return;
    std::string str;
    for(unsigned int i=0; i<8; i++){
        str+=numeroBit[i]->text().toStdString();
    }
    int i=NumBitToNum(str);
    num->setValue(i);

}
