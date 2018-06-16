#ifndef AUTOMATE_2D_H
#define AUTOMATE_2D_H

#include "Rule.h"
#include <Automaton.h>
#include <QCheckBox>
#include <QIntValidator>
#include <QLineEdit>
#include <QModelIndex>
#include <QScrollBar>
#include <QString>
#include <QTableWidget>
#include <QWidget>

/**
 * @file automate_2d.h
 * @brief Interface for a 2 dimensional automaton
 * This class helps to visualize a two dimensional automaton
 */

namespace Ui {
class Automate_2D;
}

class Automate_2D : public QWidget {
    Q_OBJECT

public:
    explicit Automate_2D(QWidget* parent = nullptr);
    ~Automate_2D();
    void setParent(QWidget* par) { parent = par; }
    void incRang() { rang++; }
    int getRang() const { return rang; }
    void init_simulation();
    /*   void setType(int a){type=a;}
    const int getType()const { return type; }*/

private:
    Ui::Automate_2D* ui;
    QWidget* parent;
    Automaton<bool, Index2D>* a;
    OuterTotalisticRule2D* r;
    int rang;
    bool sim;
    std::array<QCheckBox*, 9> survive, born;
    QTimer* timer;
    int type; // type=0 => bool, type = 1=>int

    void resizeEvent(QResizeEvent* event);

private slots:
    void setSize();
    void simulation();
    void cellActivation(const QModelIndex& index);
    void refreshGrid() const;
    void refreshRules() const;
    void next();
    void menu();
    void reset();
    void stop();
    void run();
    void load();
    void save();
    void rand();
    void rand_sym();
    void on_born_textEdited(const QString& rule);
    void on_survive_textEdited(const QString& rule);
    void check_born_i_clicked();
    void check_survive_i_clicked();
};

#endif // AUTOMATE_2D_H
