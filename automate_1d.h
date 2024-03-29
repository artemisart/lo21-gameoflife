#ifndef AUTOMATE_1D_H
#define AUTOMATE_1D_H

#include "Rule.h"
#include <Automaton.h>
#include <QCheckBox>
#include <QIntValidator>
#include <QLineEdit>
#include <QMenu>
#include <QModelIndex>
#include <QSpinBox>
#include <QString>
#include <QTableWidget>
#include <QTimer>
#include <QWidget>
#include <array>

/**
 * @file automate_1d.h
 * @brief Interface for a 1 dimensional automaton
 * This class helps to visualize a 1 dimentional automaton
 *
 */

namespace Ui {
class Automate_1D;
}

class Automate_1D : public QWidget {
    Q_OBJECT

public:
    explicit Automate_1D(QWidget* parent = nullptr);
    ~Automate_1D();
    void setParent(QWidget* par) { parent = par; }
    void incRang() { rang++; }
    int getRang() const { return rang; }
    void auto_load();

private:
    Ui::Automate_1D* ui;
    QWidget* parent;
    QIntValidator* zeroOneValidator;
    Automaton<bool, Index1D>* a;
    Rule1D* r;
    int rang;
    bool sim, begin;
    QTimer* timer;
    std::array<QCheckBox*, 8> rules;

    void resizeEvent(QResizeEvent* event);

private slots:
    void setSize();
    void simulation();
    void cellActivation(const QModelIndex& index);
    void next();
    void menu();
    void run();
    void save();
    void load();
    void rand();
    void rand_sym();
    void stop();
    void reset();
    void check_rules_i_clicked();
    void on_rule_valueChanged(int n);
};

#endif // AUTOMATE_1D_H
