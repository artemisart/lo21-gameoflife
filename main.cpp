
#include <QApplication>
#include "gameoflife.h"
#include<QTextCodec>


int main(int argc, char * argv[]) {
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QApplication app(argc, argv);
    Gameoflife fenetre;
    fenetre.show();
    return app.exec();
}

