#include "automate_1d.h"
#include "ui_automate_1d.h"
#include <QFileDialog>
#include <QMessageBox>

Automate_1D::Automate_1D(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Automate_1D)
    , rang(1)
    , sim(true)
    , begin(false)
{
    ui->setupUi(this);
    timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));

    QCheckBox* _rules[8] = {
        ui->rule0, ui->rule1, ui->rule2, ui->rule3,
        ui->rule4, ui->rule5, ui->rule6, ui->rule7
    };

    std::copy_n(_rules, 8, rules);
    ui->rule->setText(QString::fromStdString("0"));



    auto h = new RingHistory<Grid<bool, Index1D>>(10);
    r = new Rule1D();
    a = new Automaton<bool, Index1D>(h, r);

    connect(ui->sizeButton, SIGNAL(clicked()), this, SLOT(setSize()));
    connect(ui->rule, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));

    for(int i=0;i<8;i++){
        connect(rules[i], &QCheckBox::clicked, this, &Automate_1D::check_rules_i_clicked);
    }

    connect(ui->run, SIGNAL(clicked()), this, SLOT(simulation()));

    connect(ui->grid, SIGNAL(clicked(QModelIndex)), this, SLOT(cellActivation(const QModelIndex&)));

    connect(ui->next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->menu, SIGNAL(clicked()), this, SLOT(menu()));

    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(ui->load, SIGNAL(clicked(bool)), this, SLOT(load()));
    connect(ui->random, SIGNAL(clicked(bool)), this, SLOT(rand()));
    connect(ui->random_sym, SIGNAL(clicked(bool)), this, SLOT(rand_sym()));

    connect(ui->stop, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->reset, SIGNAL(clicked()), this, SLOT(reset()));

    reset();
}

Automate_1D::~Automate_1D()
{
    delete ui;
}

void Automate_1D::stop()
{
    sim = false;
}

void Automate_1D::reset()
{
    stop();

    ui->sizeSpinbox->setValue(20);
    ui->rule->setValue(30);
    ui->nb_etats->setValue(100);
    rang = 1;

    for(int i=0; i<8; i++){
        rules[i]->setChecked(false);
    }

    a->getHistory()->getStart()->iterate_set([]() { return false; });

    setSize();

    ui->grid->setEditTriggers(QAbstractItemView::DoubleClicked);
}

void Automate_1D::setSize()
{
    int dimCol = ui->sizeSpinbox->value();
    ui->grid->setRowCount(1);
    ui->grid->setColumnCount(dimCol);

    for (int i = 0; i < dimCol; i++) {
        ui->grid->setItem(0, i, new QTableWidgetItem(""));
    }
    auto* g1 = new Grid1D<bool>(dimCol);
    a->getHistory()->setStart(g1);
    resizeEvent(nullptr);
}


void Automate_1D::simulation()
{
    ui->grid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sim = true;
    run();
}

void Automate_1D::cellActivation(const QModelIndex& index)
{
    if (index.row() > 0) // this is not the initial state
        return;
    int col = index.column();
    auto start = a->getHistory()->getStart();
    start->setCell(col, start->getCell(col));
    // TODO refreshGrid();
    bool newVal = !start->getCell(col);
    ui->grid->item(0, col)->setBackgroundColor(newVal ? "black" : "white");
    start->setCell(col, newVal);
}

void Automate_1D::next()
{
    if (getRang() < ui->nb_etats->value()) {

        a->next();

        auto* grid = a->getHistory()->getLast();

        ui->grid->insertRow(ui->grid->rowCount());
        //        ui->grid->setRowCount(ui->grid->rowCount() + 1);

        for (int j = 0; j < ui->sizeSpinbox->value(); j++) {
            ui->grid->setItem(getRang(), j, new QTableWidgetItem(""));
            bool val = grid->getCell(j);
            ui->grid->item(getRang(), j)->setBackgroundColor(val ? "black" : "white");
        }
        resizeEvent(nullptr);
        incRang();
    }

}

void Automate_1D::menu()
{
    this->hide();
    this->parent->show();
}

void Automate_1D::run()
{
    if (sim == true) {
        timer->start(ui->timer->value() * 1000);
        next();
    }
}

// TODO remove this function (it appears to be unused ?)
void Automate_1D::init_simulation(int row)
{
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < ui->sizeSpinbox->value(); j++) {
            ui->grid->setColumnWidth(j, 25);
            ui->grid->setRowHeight(i, 25);
            ui->grid->setItem(i, j, new QTableWidgetItem(""));
        }
    }
    auto start = a->getHistory()->getStart();
    for (int k = 0; k < ui->sizeSpinbox->value(); k++)
        ui->grid->item(0, k)->setBackgroundColor(start->getCell(k) ? "black" : "white");
    incRang();

    begin = true;
}

void Automate_1D::save()
{
    try {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save grid"), "",
            tr("lo21 (*.1Dlo21)"));
        if (fileName.isEmpty())
            return;
        std::string name = fileName.toStdString()+".1Dlo21";
        const Grid<bool, Index1D>* g1D = a->getHistory()->getLast();
        g1D->save(name);
        r->save(name);
        std::cout << "sauvegarde reussie \n";
    } catch (const std::string& e) {
        std::cout << "erreur: " << e << "\n";
    }
}

void Automate_1D::load()
{
    if (rang > 1) {
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, si vous souhaitez réellement en créer un nouveau, appuyez sur reset, pour continuer la simulation de celui ci, appuyez de nouveau sur 'lancer la simulation'");
        msgBox.exec();
        sim = false;
    }
    try {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("load grid"), "",
            tr("lo21 (*.1Dlo21)"));
        if (fileName.isEmpty())
            return;
        std::string name = fileName.toStdString();
        Grid1D<bool>* g1D = new Grid1D<bool>(20);
        g1D->load(name);
        a->getHistory()->push(g1D);
        Index1D i = g1D->getSize();
        ui->sizeSpinbox->setValue(i.i); //met la taille a jour
        this->setSize();

        for (Index1D i; i.i < g1D->getSize().i; ++i.i) {
            bool a = g1D->getCell(i);
            ui->grid->item(0, i.i)->setBackgroundColor(a ? "black" : "white");
        }

        r->load(name);
        ui->rule->setText(QString::number(r->getNum())); //met les regles a jour
        //this->synchronizeNumToNumBit(r->getNum());
    } catch (const std::string& e) {
        std::cout << "erreur: " << e << "\n";
    }
}

void Automate_1D::rand()
{
    if (rang > 1) {
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, si vous souhaitez réellement en créer un nouveau, appuyez sur reset, pour continuer la simulation de celui ci, appuyez de nouveau sur 'lancer la simulation'");
        msgBox.exec();
        sim = false;
    }
    auto start = a->getHistory()->getStart();
    for (int j = 0; j < ui->sizeSpinbox->value(); j++) {
        int a = std::rand() % 2;
        start->setCell(j, a);
        ui->grid->item(0, j)->setBackgroundColor(a ? "black" : "white");
    }
}

void Automate_1D::rand_sym()
{
    if (rang > 1) {
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, si vous souhaitez réellement en créer un nouveau, appuyez sur reset, pour continuer la simulation de celui ci, appuyez de nouveau sur 'lancer la simulation'");
        msgBox.exec();
        sim = false;
    }
    auto start = a->getHistory()->getStart();
    auto last = start->getSize().i - 1;
    for (int j = 0; j < last / 2; j++) {
        int a = std::rand() % 2;
        start->setCell(j, a);
        start->setCell(last - j, a);
        ui->grid->item(0, j)->setBackgroundColor(a ? "black" : "white");
        ui->grid->item(0, last - j)->setBackgroundColor(a ? "black" : "white");
    }
}

void Automate_1D::resizeEvent(QResizeEvent* event)
{
    auto size = ui->grid->width() / ui->grid->columnCount();
    size = std::max(size, 5);
    for (int i = 0; i < ui->grid->columnCount(); i++) {
        ui->grid->setColumnWidth(i, size);
    }
    for (int i = 0; i < ui->grid->rowCount(); ++i) {
        ui->grid->setRowHeight(i, size);
    }

void Automate_1D::check_rules_i_clicked()
{

    QString newText;
   for(int i=0;i<8;++i){
        bool b = rules[i]->isChecked();
        int a=(int)b;
        newText+=QString::number(a);
       // ui->rule->setText(newText);

    }
    std::uint8_t a = NumBitToNum(newText);
    ui->rule->setText(QString::number(a));
    r->setNum(a);

}


void Automate_1D::synchronizeNumBitToNum(const QString& s)
{
    try {
    QString str;

    for (auto nb : numBits) {
        if (nb->text().size() == 0)
            return;
        str += nb->text();
    }

    std::uint8_t i = NumBitToNum(str);
    ui->rule->setText(QString::number(i));
    r->setNum(i);
} catch (const std::string& e) {
    std::cout << "erreur: " << e << "\n";
}
}

std::uint8_t NumBitToNum(const QString& num)
{

    int puissance = 1;
    std::uint8_t numero = 0;
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
