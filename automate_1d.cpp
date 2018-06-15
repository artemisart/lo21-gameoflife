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
    ui->size_Box->setValue(20);
    ui->rule->setValue(0);
    ui->nb_etats->setValue(10);
    rang = 1;

    a->getHistory()->getStart()->iterate_set([]() { return true; });

    setSize();

    ui->grid->setEditTriggers(QAbstractItemView::DoubleClicked);
}

void Automate_1D::setSize()
{
    int dimCol = ui->size_Box->value();
    ui->grid->setRowCount(1);
    ui->grid->setColumnCount(dimCol);

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
            ui->grid->setRowHeight(i, 25);
            ui->grid->setItem(i, j, new QTableWidgetItem(""));
        }
    }
    auto start = a->getHistory()->getStart();
    for (int k = 0; k < ui->size_Box->value(); k++)
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
        ui->size_Box->setValue(i.i); //met la taille a jour
        this->setSize();

        for (Index1D i; i.i < g1D->getSize().i; ++i.i) {
            bool a = g1D->getCell(i);
            ui->grid->item(0, i.i)->setBackgroundColor(a ? "black" : "white");
        }

        r->load(name);
        ui->rule->setValue(r->getNum()); //met les regles a jour
        this->synchronizeNumToNumBit(r->getNum());
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
    for (int j = 0; j < ui->size_Box->value(); j++) {
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
