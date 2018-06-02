#ifndef AUTOCELL
#define AUTOCELL
/*
#include <QString>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>


class AutoCell : public QWidget{
    Q_OBJECT
    QSpinBox* num; // numéro
    QLineEdit* numeroBit[8]; // un QLineEdit par bit
    QLabel* numl;
    QLabel* numeroBitl[8];
    QVBoxLayout* numc;
    QVBoxLayout* bitc[8];
    QHBoxLayout* numeroc;
    QVBoxLayout*couche;
    QIntValidator* zeroOneValidator;
    QTableWidget* depart;
    QPushButton * simulation;
    QTableWidget * etats;
    static unsigned int dimension;
    public:
    explicit AutoCell(QWidget* parent = nullptr);
    private slots:
    void synchronizeNumToNumBit(int i);
    void synchronizeNumBitToNum(const QString& s);
    void faireSimulation();
    void cellActivation(const QModelIndex& index);
}; */

#endif // AUTOCELL

