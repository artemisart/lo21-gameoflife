#include <QApplication>

#include "welcome.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Welcome fenetre;
    fenetre.show();
    return app.exec();

    // vvv - test here - vvv
    // instantiating templates classes force them to be compiled, and so we can
    // check errors
    /*
    auto* h = new RingHistory<Grid<bool, Index1D>>(5);
    auto* r = new Rule1D();
    auto* a = new Automaton<bool, Index1D>(h, r);
    auto* g1 = new Grid1D<bool>(10);
    h->setStart(*g1);

    a->next();
    a->run(2);

    auto* g2 = new Grid2D<bool>(20, 20);

    r->setNum(2);
    r->calcNextState(*g1, Index1D(2));
    g2->setCell(Index2D(3, 4), true);
    h->push(g1);
    h->getLast();

    delete g2;
    delete g1;
    delete a;
    delete h;
    delete r;

    auto func = [](int a) {
        return a >= 0;
    };

    func(1);

    std::function<int(int)> _func = func; */
}
