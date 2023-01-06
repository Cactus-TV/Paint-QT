#include <vector>
//выбранная фигура
class Figure;
Figure* SELECTED_FIGURE;
std::vector<Figure*> figures;

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1000,1000);
    w.show();
    return a.exec();
}
