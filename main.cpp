
#include <QApplication>
#include "gameoflife.h"
#include<QTextCodec>


int main(int argc, char * argv[]) {
    QApplication app(argc, argv);
    Gameoflife fenetre;
    fenetre.show();
    return app.exec();
}

