#include "automate_2d.h"
#include "automate_1d.h"
#include "ui_automate_2d.h"
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

	QCheckBox* _survive[9] = {
		ui->survive0, ui->survive1, ui->survive2, ui->survive3,
		ui->survive4, ui->survive5, ui->survive6, ui->survive7, ui->survive8
    };
	QCheckBox* _born[9] = {
		ui->born0, ui->born1, ui->born2, ui->born3,
		ui->born4, ui->born5, ui->born6, ui->born7, ui->born8
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
		connect(born[i], SIGNAL(textEdited(QString)), this, SLOT(synchronizeNumBitToNum_b(QString)));
		connect(survive[i], SIGNAL(textEdited(QString)), this, SLOT(synchronizeNumBitToNum_s(QString)));
    }

	connect(ui->size_b, SIGNAL(clicked()), this, SLOT(setSize()));

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

void Automate_2D::reset()
{
    ui->largeur->setValue(20);
    ui->hauteur->setValue(20);
	ui->survive->setText("23");
	ui->born->setText("2");
    setSize();

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
            start->setCell(Index2D(i,j), true);
            ui->grid->item(i,j)->setText("");
        }
    }
    ui->grid->setEditTriggers(QAbstractItemView::DoubleClicked);

}

void Automate_2D::stop()
{
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

void Automate_2D::synchronizeNumBitToNum_s(const QString& s)
{
	// TODO remove this function
    QString str;

	for (int k = 0; k < 9; k++) {
		if (survive[k]->text() == "")
			return;
		str += survive[k]->text();
	}

	std::uint16_t i = NumBitToNum(str);
	//	ui->Survive->setValue(i);
	r->setSurvive(i);
}

void Automate_2D::synchronizeNumBitToNum_b(const QString& s)
{
	// TODO remove this function
    QString str;

	for (unsigned int k = 0; k < 9; k++) {
		if (born[k]->text() == "")
			return;
		str += born[k]->text();
    }

	std::uint16_t i = NumBitToNum(str);
    //ui->born->setValue(i);
	ui->born->setText(str);
    r->setBorn(i);
}

void Automate_2D::simulation()
{

	sim = true;
    run();
}

void Automate_2D::run()
{
	if (sim == true) {
		timer->start(ui->timer->value() * 1000);

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

}

void Automate_2D::load(){

}


void Automate_2D::rand()
{
	for (int j = 0; j < ui->hauteur->value(); j++) {
		for (int i = 0; i < ui->largeur->value(); i++) {
			int a = std::rand() % 2;
			if (a == 0) {
				ui->grid->item(j, i)->setBackgroundColor("white");
				start->setCell(Index2D(j, i), false);
            }
            else {
				ui->grid->item(j, i)->setBackgroundColor("black");
				start->setCell(Index2D(j, i), true);
            }
        }

    }

}

void Automate_2D::rand_sym()
{
	int h = ui->hauteur->value();
	for (int j = 0; j < h / 2; j++) {
		for (int i = 0; i < ui->largeur->value(); i++) {
			int a = std::rand() % 2;
			ui->grid->item(j, i)->setBackgroundColor(a ? "black" : "white");
			start->setCell(Index2D(j, i), a);
		}

    }



    }
	//	int half = (int)std::ceil(ui->hauteur->value() / 2.) - 1;
	int half = ui->hauteur->value() / 2; // TODO check if this is sufficient
	int i = 0;
	for (int j = 1; j <= half + 1; j++) {
		for (int k = 0; k < ui->largeur->value(); k++) {

			if (ui->grid->item(half - i, k)->backgroundColor() == "white") {
				ui->grid->item(half + j, k)->setBackgroundColor("white");
                start->setCell(Index2D(half+j,k), false);

            }else{
                ui->grid->item( half+j, k)->setBackgroundColor("black");
                start->setCell(Index2D(half+j,k), true);



            }
        }
            i++;
       }


}
void Automate_2D::on_born_textEdited(const QString& str)
{
	QString newText;
	std::uint16_t rule = 0;
	for (int i = 0; i < 9; ++i) {
		bool b = str.contains('0' + i);
		born[i]->setChecked(b);
		rule += b << i;
		if (b)
			newText.append('0' + i);
	}
	ui->born->setText(newText);
	r->setBorn(rule);
}

void Automate_2D::on_survive_textEdited(const QString& str)
{
	QString newText;
	std::uint16_t rule = 0;
	for (int i = 0; i < 9; ++i) {
		bool b = str.contains('0' + i);
		survive[i]->setChecked(b);
		rule += b << i;
		if (b)
			newText.append('0' + i);
	}
	ui->survive->setText(newText);
	r->setSurvive(rule);
}

void Automate_2D::on_born_i_stateChanged()
{
	QString newText;
	std::uint16_t rule = 0;
	for (int i = 0; i < 9; ++i) {
		bool b = born[i]->isChecked();
		rule += b << i;
		if (b)
			newText.append('0' + i);
	}
	ui->born->setText(newText);
	r->setBorn(rule);
}
