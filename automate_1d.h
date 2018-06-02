#ifndef AUTOMATE_1D_H
#define AUTOMATE_1D_H

#include <QWidget>
#include<QModelIndex>

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

private slots:
     void setSize();
     void synchronizeNumToNumBit(int j);
     void synchronizeNumBitToNum(const QString& s);
     void simulation();
     void cellActivation(const QModelIndex& index);
     void next();
     void menu();

};

std::string NumToNumBit(short unsigned int num);
short unsigned int NumBitToNum(const std::string& num);




#endif // AUTOMATE_1D_H
