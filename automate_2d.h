#ifndef AUTOMATE_2D_H
#define AUTOMATE_2D_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class Automate_2D;
}

class Automate_2D : public QWidget
{
    Q_OBJECT

public:
    explicit Automate_2D(QWidget *parent = 0);
    ~Automate_2D();
    void setParent(QWidget* par){parent=par;}


private:
    Ui::Automate_2D *ui;
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


#endif // AUTOMATE_2D_H
