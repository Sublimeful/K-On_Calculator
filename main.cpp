#include "calc.h"

#include <QApplication>
#include <QObject>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QPushButton {"
"background-color: rgba(77,82,255,0.5);"
"color: rgb(0,221,255);"
"font: 700 18pt 'Segoe Ui';"
"}");
    Calc w;
    w.show();

    return a.exec();
}
