#include "automate_2d.h"
#include "automate_1d.h"
#include "ui_automate_2d.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QTimer>

Automate_2D::Automate_2D(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Automate_2D)
    , rang(0)
    , sim(true)
{
    ui->setupUi(this);

    timer = new QTimer(parent);
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));

    survive = { { ui->survive0, ui->survive1, ui->survive2,
        ui->survive3, ui->survive4, ui->survive5,
        ui->survive6, ui->survive7, ui->survive8 } };
    born = { { ui->born0, ui->born1, ui->born2,
        ui->born3, ui->born4, ui->born5,
        ui->born6, ui->born7, ui->born8 } };

    //if(this->getType() == 0)
    auto h = new RingHistory<Grid<bool, Index2D>>(10);
    // if(this->getType()== 1) h = new RingHistory<Grid<uint8_t, Index2D> >(10);
    r = new Rule2D();
    a = new Automaton<bool, Index2D>(h, r);

    setSize();
    refreshGrid();

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
    ui->grid->setMinimumSize(25 * dimRow, 25 * dimCol);

    ui->grid->setColumnCount(dimCol);
    ui->grid->setRowCount(dimRow);

    for (int i = 0; i < dimCol; i++) {
        ui->grid->setColumnWidth(i, 25);
        for (int j = 0; j < dimRow; j++) {
            ui->grid->setRowHeight(j, 25);
            ui->grid->setItem(j, i, new QTableWidgetItem(""));
        }
    }
    auto* g1 = new Grid2D<bool>(dimRow, dimCol);
    a->getHistory()->setStart(g1);
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
    this->hide();
    this->parent->show();
}

void Automate_2D::save()
{
    try {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("save grid"), "",
            tr("lo21 (*.2Dlo21)"));
        if (fileName.isEmpty())
            return;
        std::string name = fileName.toStdString() + ".2Dlo21";
        const Grid<bool, Index2D>* g2D = a->getHistory()->getLast();
        g2D->save(name);
        r->save(name);
        std::cout << "sauvegarde reussie \n";
    } catch (const std::string& e) {
        std::cout << "erreur: " << e << "\n";
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
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("load grid"), "",
            tr("lo21 (*.2Dlo21)"));
        if (fileName.isEmpty())
            return;
        std::string name = fileName.toStdString();
        Grid2D<bool>* g2D = new Grid2D<bool>(10, 10);
        g2D->load(name);
        a->getHistory()->push(g2D);
        Index2D i = g2D->getSize();
        ui->widthSpinbox->setValue(i.col);
        ui->heightSpinbox->setValue(i.row);
        this->setSize();

        for (Index2D i; i.row < g2D->getSize().row; ++i.row) {
            for (i.col = 0; i.col < g2D->getSize().col; ++i.col) {
                bool a = g2D->getCell(i);
                if (a == 0) {
                    ui->grid->item(i.row, i.col)->setBackgroundColor("white");
                    ui->grid->item(i.row, i.col)->setText("");
                    ui->grid->item(i.row, i.col)->setBackgroundColor("white");
                } else {
                    ui->grid->item(i.row, i.col)->setBackgroundColor("black");
                    ui->grid->item(i.row, i.col)->setText("_");
                    ui->grid->item(i.row, i.col)->setBackgroundColor("black");
                }
            }
        }

        r->load(name);
        // TODO update rules
        //        ui->survive->setValue(r->getSurvive());
        //        ui->born->setValue(r->getBorn());
        //        this->synchronizeNumToNumBit_b(r->getBorn());
        //        this->synchronizeNumToNumBit_s(r->getSurvive());
        std::cout << "loading reussi";
    } catch (const std::string& e) {
        std::cout << "erreur: " << e << "\n";
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
    int h = ui->heightSpinbox->value();
    auto start = a->getHistory()->getStart();

    for (int j = 0; j < h / 2; j++) {
        for (int i = 0; i < ui->widthSpinbox->value(); i++) {
            int val = std::rand() % 2;
            ui->grid->item(j, i)->setBackgroundColor(val ? "black" : "white");
            start->setCell(Index2D(j, i), val);
        }
    }
    int half = (int)std::ceil(ui->heightSpinbox->value() / 2.) - 1;

    int i = 0;
    for (int j = 1; j <= half + 1; j++) {
        for (int k = 0; k < ui->widthSpinbox->value(); k++) {

            if (ui->grid->item(half - i, k)->backgroundColor() == "white") {
                ui->grid->item(half + j, k)->setBackgroundColor("white");
                start->setCell(Index2D(half + j, k), false);

            } else {
                ui->grid->item(half + j, k)->setBackgroundColor("black");
                start->setCell(Index2D(half + j, k), true);
            }
        }
        i++;
    }
}

void Automate_2D::on_born_textEdited(const QString& str)
{
    QString newText;
    std::uint16_t rule = 0;
    for (size_t i = 0; i < 9; ++i) {
        bool b = str.contains('0' + i);
        born[i]->setChecked(b);
        if (b) {
            rule += 1 << i;
            newText.append('0' + i);
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
        bool b = str.contains('0' + i);
        survive[i]->setChecked(b);
        if (b) {
            rule += 1 << i;
            newText.append('0' + i);
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
            newText.append('0' + i);
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
            newText.append('0' + i);
        }
    }
    ui->survive->setText(newText);
    r->setSurvive(rule);
}
