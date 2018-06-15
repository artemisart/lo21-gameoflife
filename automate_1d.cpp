#include "automate_1d.h"
#include "ui_automate_1d.h"
#include <QMessageBox>

Automate_1D::Automate_1D(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Automate_1D)
    , rang(1)
    , sim(true)
    , begin(false)
{
    timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));

    ui->setupUi(this);
    numBits = { { ui->numBit1, ui->numBit2, ui->numBit3, ui->numBit4,
        ui->numBit5, ui->numBit6, ui->numBit7, ui->numBit8 } };

    auto h = new RingHistory<Grid<bool, Index1D>>(10);
    r = new Rule1D();
    a = new Automaton<bool, Index1D>(h, r);

    // TODO checkboxes here
    zeroOneValidator = new QIntValidator(0, 1, this);
    for (auto nb : numBits) {
        nb->setValidator(zeroOneValidator);
        connect(nb, SIGNAL(textChanged(QString)), this, SLOT(synchronizeNumBitToNum(QString)));
    }

    setSize();

    connect(ui->size, SIGNAL(clicked()), this, SLOT(setSize()));
    connect(ui->rule, SIGNAL(valueChanged(int)), this, SLOT(synchronizeNumToNumBit(int)));

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
    ui->grid->setRowCount(1);
    ui->size_Box->setValue(20);
    ui->rule->setValue(0);
    ui->nb_etats->setValue(10);
    rang=1;

    a->getHistory()->getStart()->iterate_set([]() { return true; });

    setSize();

    ui->grid->setEditTriggers(QAbstractItemView::DoubleClicked);
}

void Automate_1D::setSize()
{
    int dimCol = ui->size_Box->value();
    ui->grid->setColumnCount(dimCol);

    ui->grid->horizontalHeader()->setVisible(false);
    ui->grid->verticalHeader()->setVisible(false);
    ui->grid->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->grid->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->grid->setFixedWidth(ui->size_Box->value() * 25);

    for (int i = 0; i < dimCol; i++) {
        ui->grid->setColumnWidth(i, 25);
        ui->grid->setItem(0, i, new QTableWidgetItem(""));
    }
    auto* g1 = new Grid1D<bool>(dimCol);
    a->getHistory()->setStart(g1);
}

std::uint8_t NumBitToNum(const QString& num)
{
    if (num.size() != 8)
        // FIXME cette condition va pas du tout, dès qu'on a 00 ou 01 dans un champ ça fait tout crasher
        throw("Numero d'automate indefini");
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

void Automate_1D::synchronizeNumBitToNum(const QString& s)
{
    QString str;

    for (auto nb : numBits) {
        if (nb->text().size() == 0)
            return;
        str += nb->text();
    }

    std::uint8_t i = NumBitToNum(str);
    ui->rule->setValue(i);
    r->setNum(i);
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

        ui->grid->setRowCount(ui->grid->rowCount() + 1);
        ui->grid->setFixedSize(ui->size_Box->value() * 25, ((ui->grid->rowCount() + 1) * 25));

        for (int j = 0; j < ui->size_Box->value(); j++) {
            ui->grid->setItem(getRang(), j, new QTableWidgetItem(""));
            bool val = grid->getCell(j);
            ui->grid->item(getRang(), j)->setBackgroundColor(val ? "black" : "white");
        }
        incRang();

    } else {
        QMessageBox msgBox;
        msgBox.setText("Le nombres d'états maximal de la simulation à été atteint");
        msgBox.exec();
        sim = false;
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

void Automate_1D::init_simulation(int row)
{

    for (int i = 0; i < row; i++) {

        for (int j = 0; j < ui->size_Box->value(); j++) {
            ui->grid->setColumnWidth(j, 25);
            etats->setRowHeight(i, 25);
            etats->setItem(i, j, new QTableWidgetItem(""));
        }
    }
    auto start = a->getHistory()->getStart();
    for (int k = 0; k < ui->size_Box->value(); k++)
        etats->item(0, k)->setBackgroundColor(start->getCell(k) ? "black" : "white");
    incRang();

    ui->interface_2->addWidget(etats);
    ui->interface_2->removeWidget(ui->grid);

    begin = true;
}

void Automate_1D::save()
{
    std::string name = ui->name_file->text().toStdString();
    const Grid<bool, Index1D>* g1D = a->getHistory()->getLast();
    g1D->save(name);
    r->save(name);
    std::cout << "sauvegarde reussie \n";
}

void Automate_1D::load()
{
    if(rang >1 ){
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, si vous souhaitez réellement en créer un nouveau, appuyez sur reset, pour continuer la simulation de celui ci, appuyez de nouveau sur 'lancer la simulation'");
        msgBox.exec();
        sim = false;

    }
    std::string name = ui->name_file->text().toStdString();
    Grid1D<bool>* g1D = new Grid1D<bool>(20);
    g1D->load(name);
    a->getHistory()->push(g1D);
    r->load(name);
}

void Automate_1D::rand()
{
    if(rang >1 ){
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, si vous souhaitez réellement en créer un nouveau, appuyez sur reset, pour continuer la simulation de celui ci, appuyez de nouveau sur 'lancer la simulation'");
        msgBox.exec();
        sim = false;

    }
    auto start = a->getHistory()->getStart();
    for (int j = 0; j < ui->size_Box->value(); j++) {
        int a = std::rand() % 2;
        if (a == 0) {
            ui->grid->item(0, j)->setBackgroundColor("white");
            start->setCell(j, false);
        } else {
            ui->grid->item(0, j)->setBackgroundColor("black");
            start->setCell(j, true);
        }
    }
}

void Automate_1D::rand_sym()
{
    if(rang >1 ){
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, si vous souhaitez réellement en créer un nouveau, appuyez sur reset, pour continuer la simulation de celui ci, appuyez de nouveau sur 'lancer la simulation'");
        msgBox.exec();
        sim = false;

    }
    auto start = a->getHistory()->getStart();
    for (int j = 0; j < (ui->size_Box->value()) / 2; j++) {
        int a = std::rand() % 2;
        if (a == 0) {
            ui->grid->item(0, j)->setBackgroundColor("white");
            start->setCell(j, false);
            ui->grid->item(0, j)->setText("");
            ui->grid->item(0, j)->setBackgroundColor("white");
        } else {
            ui->grid->item(0, j)->setBackgroundColor("black");
            start->setCell(j, true);
            ui->grid->item(0, j)->setText("_");
            ui->grid->item(0, j)->setBackgroundColor("black");
        }
    }
    int half = std::ceil((ui->size_Box->value()) / 2) - 1;
    //int half = ui->size_Box->value() / 2;
    int i = 0;
    for (int j = 1; j <= half + 1; j++) {
        if (ui->grid->item(0, half - i)->text() == "") {
            ui->grid->item(0, half + j)->setBackgroundColor("white");
            start->setCell(j, false);

        } else {
            ui->grid->item(0, half + j)->setBackgroundColor("black");
            start->setCell(j, true);
        }
        i++;
    }
}
