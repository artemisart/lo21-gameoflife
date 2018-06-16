#include "automate_2d.h"
#include "automate_1d.h"
#include "ui_automate_2d.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QTimer>
#include<stdio.h>
#include<stdlib.h>

Automate_2D::Automate_2D(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Automate_2D)
    , rang(0)
    , sim(true)
{
    ui->setupUi(this);

    //initialize timer
    timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));

    survive = { { ui->survive0, ui->survive1, ui->survive2,
        ui->survive3, ui->survive4, ui->survive5,
        ui->survive6, ui->survive7, ui->survive8 } };
    born = { { ui->born0, ui->born1, ui->born2,
        ui->born3, ui->born4, ui->born5,
        ui->born6, ui->born7, ui->born8 } };

    //initialize parameters to make interface work with the application
    auto h = new RingHistory<Grid<bool, Index2D>>(10);
    // if(this->getType()== 1) h = new RingHistory<Grid<uint8_t, Index2D> >(10);
    r = new OuterTotalisticRule2D();
    a = new Automaton<bool, Index2D>(h, r);

    //connect all differents slots

    for (size_t i = 0; i < 9; i++) {
        connect(born[i], &QCheckBox::clicked, this, &Automate_2D::check_born_i_clicked);
        connect(survive[i], &QCheckBox::clicked, this, &Automate_2D::check_survive_i_clicked);
    }

    connect(ui->sizeButton, SIGNAL(clicked()), this, SLOT(setSize()));

    connect(ui->run, SIGNAL(clicked()), this, SLOT(simulation()));

    connect(ui->grid, SIGNAL(clicked(QModelIndex)), this, SLOT(cellActivation(QModelIndex)));

    connect(ui->next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->menu, SIGNAL(clicked()), this, SLOT(menu()));
    connect(ui->reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->stop, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->save, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->load, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->random, SIGNAL(clicked()), this, SLOT(rand()));
    connect(ui->random_sym, SIGNAL(clicked()), this, SLOT(rand_sym()));

    reset();
    auto_load();
}

Automate_2D::~Automate_2D()
{
    delete ui;
}

void Automate_2D::reset()
{
    stop();

    ui->widthSpinbox->setValue(20);
    ui->heightSpinbox->setValue(20);
    ui->survive->setText("23");
    on_survive_textEdited("23");
    ui->born->setText("3");
    on_born_textEdited("3");
    rang = 1;

    setSize();

    refreshGrid();
    ui->grid->setEditTriggers(QAbstractItemView::DoubleClicked);
}

void Automate_2D::stop()
{
    sim = false;
}

void Automate_2D::setSize()
{
    if (rang > 1) {
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, en continiant vous allez arrêter l'automate en cours'");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Cancel:
            return;
        default:
            break;
        }
    }
    int dimRow = ui->heightSpinbox->value();
    int dimCol = ui->widthSpinbox->value();

    ui->grid->setColumnCount(dimCol);
    ui->grid->setRowCount(dimRow);

    for (int i = 0; i < dimCol; i++) {
        for (int j = 0; j < dimRow; j++) {
            ui->grid->setItem(j, i, new QTableWidgetItem(""));
        }
    }
    auto* g1 = new Grid2D<bool>(dimRow, dimCol);
    a->getHistory()->setStart(g1);
    resizeEvent(nullptr);
}

void Automate_2D::simulation()
{
    sim = true;
    run();
}

void Automate_2D::run()
{
    if (sim) {
        timer->start(ui->timer->value() * 1000);
        next();
    }
}

void Automate_2D::cellActivation(const QModelIndex& index)
{
    Index2D i(index.row(), index.column());
    auto grid = const_cast<Grid<bool, Index2D>*>(a->getHistory()->getLast());
    grid->setCell(i, !grid->getCell(i));
    refreshGrid();
}

void Automate_2D::next()
{
    ui->grid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    a->next();
    refreshGrid();
    incRang();
}

void Automate_2D::menu()
{
    Grid2D<bool>* g2D = new Grid2D<bool>(ui->heightSpinbox->value(), ui->widthSpinbox->value());
    for (int i = 0; i < ui->heightSpinbox->value(); i++) {
        for (int j = 0; j < ui->widthSpinbox->value(); j++) {

            if (ui->grid->item(i, j)->backgroundColor() == "white")
                g2D->setCell(Index2D(i, j), false);
            else {
                g2D->setCell(Index2D(i, j), true);
            }
        }
    }
    g2D->save("config.2Dlo21");
    r->save("config.2Dlo21");

    this->hide();
    this->parent->show();
}

void Automate_2D::save()
{
    try {
        QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Sauvegarder la configuration actuelle"),
            "",
            tr("Grille LO21 (*.2Dlo21)"));
        if (fileName.isEmpty())
            return;

        std::string name = fileName.toStdString();
        auto* g2D = a->getHistory()->getLast();
        g2D->save(name);
        r->save(name);
    } catch (const std::exception& err) {
        std::cerr << "erreur: " << err.what() << "\n";
        QMessageBox::critical(
            this,
            tr("Erreur lors de l'enregistrement"),
            err.what());
    }
}

void Automate_2D::load()
{
    if (rang > 1) {
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, si vous souhaitez réellement en créer un nouveau, appuyez sur reset, pour continuer la simulation de celui ci, appuyez de nouveau sur 'lancer la simulation'");
        msgBox.exec();
        sim = false;
        return;
    }
    try {
        QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Charger une configuration"),
            "",
            tr("Grille LO21 (*.2Dlo21)"));
        if (fileName.isEmpty())
            return;

        std::string name = fileName.toStdString();
        Grid2D<bool>* g2D = new Grid2D<bool>(10, 10);
        g2D->load(name);

        Index2D i = g2D->getSize();
        ui->widthSpinbox->setValue(i.col);
        ui->heightSpinbox->setValue(i.row);
        this->setSize();
        a->getHistory()->setStart(g2D);
        refreshGrid();
        r->load(name);
        refreshRules();
    } catch (const std::exception& err) {
        std::cerr << "erreur: " << err.what() << "\n";
        QMessageBox::critical(
            this,
            tr("Erreur lors du chargement"),
            err.what());
    }
}
void Automate_2D::rand()
{
    if (rang > 1) {
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, si vous souhaitez réellement en créer un nouveau, appuyez sur reset, pour continuer la simulation de celui ci, appuyez de nouveau sur 'lancer la simulation'");
        msgBox.exec();
        sim = false;
    }
    a->getHistory()->getStart()->iterate_set([]() {
        return std::rand() % 2;
    });
    refreshGrid();
}

void Automate_2D::refreshGrid() const
{
    auto grid = this->ui->grid;
    a->getHistory()->getLast()->iterate_get([&grid](const Index2D i, const bool val) {
        grid->item(i.row, i.col)->setBackgroundColor(val ? "black" : "white");
    });
}

// TODO refactor this
void Automate_2D::rand_sym()
{
    if (rang > 1) {
        QMessageBox msgBox;
        msgBox.setText("Un automate est déjà en cours de simulation, si vous souhaitez réellement en créer un nouveau, appuyez sur reset, pour continuer la simulation de celui ci, appuyez de nouveau sur 'lancer la simulation'");
        msgBox.exec();
        sim = false;
    }
    auto start = a->getHistory()->getStart();
    auto height = start->getSize().row;
    auto width = start->getSize().col;
    for (int j = 0; j < height / 2; j++) {
        for (int i = 0; i < width; i++) {
            int val = std::rand() % 2;
            start->setCell(Index2D(j, i), val);
            start->setCell(Index2D(height - 1 - j, i), val);
        }
    }
    refreshGrid();
}

void Automate_2D::on_born_textEdited(const QString& str)
{
    QString newText;
    std::uint16_t rule = 0;
    for (size_t i = 0; i < 9; ++i) {
       // bool b = str.contains('0' +QString::number(i));
        bool b = str.contains(static_cast<char>('0' +i));

        born[i]->setChecked(b);
        if (b) {
            rule += 1 << i;
            newText.append(static_cast<char>('0' + i));
        }
    }
    ui->born->setText(newText);
    r->setBorn(rule);
}

void Automate_2D::on_survive_textEdited(const QString& str)
{
    QString newText;
    std::uint16_t rule = 0;
    for (size_t i = 0; i < 9; ++i) {
        bool b = str.contains(static_cast<char>('0' + i));
        survive[i]->setChecked(b);
        if (b) {
            rule += 1 << i;
            newText.append(static_cast<char>('0' + i));
        }
    }
    ui->survive->setText(newText);
    r->setSurvive(rule);
}

void Automate_2D::check_born_i_clicked()
{
    QString newText;
    std::uint16_t rule = 0;
    for (size_t i = 0; i < 9; ++i) {
        bool b = born[i]->isChecked();
        if (b) {
            rule += 1 << i;
            newText.append(static_cast<char>('0' + i));
        }
    }
    ui->born->setText(newText);
    r->setBorn(rule);
}

void Automate_2D::check_survive_i_clicked()
{
    QString newText;
    std::uint16_t rule = 0;
    for (size_t i = 0; i < 9; ++i) {
        bool b = survive[i]->isChecked();
        if (b) {
            rule += 1 << i;
            newText.append(static_cast<char>('0' + i));
        }
    }
    ui->survive->setText(newText);
    r->setSurvive(rule);
}

void Automate_2D::refreshRules() const
{
    QString bt, st;
    auto b = r->getBorn(), s = r->getSurvive();
    for (size_t i = 0; i < 9; ++i) {
        born[i]->setChecked(b >> i & 1);
        if (b >> i & 1)
            bt.append(static_cast<char>('0' + i));
        survive[i]->setChecked(s >> i & 1);
        if (s >> i & 1)
            st.append(static_cast<char>('0' + i));
    }
    ui->born->setText(bt);
    ui->survive->setText(st);
}

void Automate_2D::resizeEvent(QResizeEvent* event)
{
    auto sizeW = (ui->grid->width() - ui->grid->verticalScrollBar()->width()) / ui->grid->columnCount();
    auto sizeH = (ui->grid->height() - ui->grid->horizontalScrollBar()->height()) / ui->grid->rowCount();
    auto size = std::max(std::min(sizeW, sizeH), 5);

    for (int i = 0; i < ui->grid->columnCount(); ++i) {
        ui->grid->setColumnWidth(i, size);
    }
    for (int i = 0; i < ui->grid->rowCount(); ++i) {
        ui->grid->setRowHeight(i, size);
    }
}

void Automate_2D::auto_load()
{
    Grid2D<bool>* g2D = new Grid2D<bool>(10, 10);
    g2D->load("config.2Dlo21");

    Index2D i = g2D->getSize();
    ui->widthSpinbox->setValue(i.col);
    ui->heightSpinbox->setValue(i.row);
    this->setSize();
    a->getHistory()->setStart(g2D);
    refreshGrid();
    r->load("config.2Dlo21");
    refreshRules();
}
