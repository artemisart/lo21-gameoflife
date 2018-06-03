#ifndef AUTOMATE_1D_H
#define AUTOMATE_1D_H

#include <QWidget>
#include<QModelIndex>
#include<QString>
#include<QIntValidator>

namespace Ui {
class Automate_1D;
}

class Automate_1D : public QWidget
{
    Q_OBJECT

public:
    explicit Automate_1D(QWidget *parent = 0);
    ~Automate_1D();
    void setParent(QWidget* par){parent=par;}

private:
    Ui::Automate_1D *ui;
    QWidget* parent;
    QIntValidator* zeroOneValidator;

private slots:
     void setSize();
     void synchronizeNumToNumBit(int j);
     void synchronizeNumBitToNum(const QString& s);
     void simulation();
     void cellActivation(const QModelIndex& index);
     void next();
     void menu();

};

QString NumToNumBit(short unsigned int num);
short unsigned int NumBitToNum(const QString& num);




#endif // AUTOMATE_1D_H
