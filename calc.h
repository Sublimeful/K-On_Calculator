#ifndef CALC_H
#define CALC_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class Calc; }
QT_END_NAMESPACE

class Calc : public QMainWindow
{
    Q_OBJECT

public:
    static bool isOperand(QChar c);
    QString equation = "";
    QString get_answer();
    Calc(QWidget *parent = nullptr);
    ~Calc();

private:
    Ui::Calc *ui;

public slots:
    void pressed(int button_type);
};
#endif // CALC_H
