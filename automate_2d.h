#ifndef AUTOMATE_2D_H
#define AUTOMATE_2D_H

#include <QIntValidator>
#include <QModelIndex>
#include <QWidget>

namespace Ui {
class Automate_2D;
}

class Automate_2D : public QWidget {
    Q_OBJECT

public:
	explicit Automate_2D(QWidget* parent = nullptr);
    ~Automate_2D();
	void setParent(QWidget* par) { parent = par; }

private:
	Ui::Automate_2D* ui;
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
short unsigned int NumBitToNum(const std::string& num);

#endif // AUTOMATE_2D_H
