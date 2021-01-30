#include "calc.h"

#include <QApplication>
#include <QObject>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QPushButton {"
                        "background-color: rgba(3,140,252,0.5);"
                        "color: rgb(3,219,252);"
                        "font: 700 18pt 'Segoe Ui';"
                    "}");
    Calc w;
    w.show();

    return a.exec();
}
