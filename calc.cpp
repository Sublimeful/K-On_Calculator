#include "calc.h"
#include "ui_calc.h"
#include <QSignalMapper>
#include <QRegularExpression>

QString Calc::get_answer() {
    QString equation = this->equation;
    if(equation == "") return "0";
    if(!equation.at(0).isDigit() || !equation.at(equation.size()-1).isDigit()) return "ERROR";
    bool equationIsNumber = true;
    for(int i=0; i < equation.size(); ++i)
    {
        if(equation.at(i) == '.' && (!equation.at(i-1).isDigit() || !equation.at(i+1).isDigit())) return "ERROR";
        if(equation.at(i) == '/' || equation.at(i) == '*' || equation.at(i) == '-' ||
           equation.at(i) == '+' || equation.at(i) == '%') equationIsNumber = false;

    } /* checks if equation is empty, or first or last is digit, and valid decimals */
    if(equationIsNumber) return QString::number(equation.toDouble());
    QVector<double> nums;
    QVector<QChar> operands;
    for(int i=0; i < equation.size(); ++i)
    {
        if(equation.at(i) == '/' || equation.at(i) == '*' || equation.at(i) == '-' ||
           equation.at(i) == '+' || equation.at(i) == '%')
        {
            if(equation.at(i) == equation.at(i+1)) return "ERROR"; //returns error if more than one op together
            //target first occurance of operation
            if(nums.size() == 0)
            {
                nums.push_back(equation.left(i).toDouble());
            }

            QString right = equation.right(equation.size() - i);
            QRegularExpression re("\\d*(\\d(?=[/*\\-+%])|$|\\.)\\d*");
            QRegularExpressionMatch match = re.match(right);

            nums.push_back(match.captured().toDouble());
            operands.push_back(equation.at(i));
        }
    }
    for(int i=0; i < operands.size(); ++i)
    {
        if(operands[i] == '*')
        {
            nums[i+1] = nums[i] * nums[i+1];
            nums[i] = 0;
        }
        else if(operands[i] == '/')
        {
            nums[i+1] = nums[i] / nums[i+1];
            nums[i] = 0;
        }
        else if(operands[i] == '%')
        {
            nums[i+1] = std::fmod(nums[i], nums[i+1]);
            nums[i] = 0;
        }
    }
    for(int i=0; i < operands.size(); ++i)
    {
        if(operands[i] == '*' || operands[i] == '/' || operands[i] == '%') continue;
        else
        {
            int j = i;
            while((i+1) != operands.size() && (operands[i+1] == '*' || operands[i+1] == '/'
                                            || operands[i+1] == '%'))
            {
                ++i;
            }
            if(operands[j] == '+')
            {
                nums[i+1] = nums[j] + nums[i+1];
            }
            else
            {
                nums[i+1] = nums[j] - nums[i+1];
            }
        }
    }
    return QString::number(nums[nums.size()-1]);
};

void Calc::pressed(int button_type)
{
    QString btn_values[] = {
        "1", //0
        "5", //1
        "+", //2
        "7", //3
        "0", //4
        "DELETE", //5
        "4", //6
        "-", //7
        "6", //8
        ".", //9
        "9", //10
        "CLEAR", //11
        "=", //12
        "2", //13
        "8", //14
        "3", //15
        "%", //16
        "*", //17
        "/", //18
    };
    if(button_type != 5 && button_type != 11 && button_type != 12)
    {
        this->equation += btn_values[button_type];
    }
    else
    {
        if(button_type == 5)
        {
            this->equation.chop(1);
        }
        else if(button_type == 11)
        {
            this->equation.clear();
        }
        else if(button_type == 12)
        {
            this->ui->Number->display(this->get_answer());
        }
    }
    this->ui->Equation->setText(this->equation);
}

Calc::Calc(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calc)
{
    ui->setupUi(this);
    QGridLayout* layout = ui->gridLayout;
    QSignalMapper* signalMapper = new QSignalMapper(this);
    for (int i = 0; i < layout->count(); ++i)
    {
        QPushButton* btn = (QPushButton*) layout->itemAt(i)->widget();
        connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(btn, i);
    }
    connect(signalMapper, SIGNAL(mappedInt(int)), this, SLOT(pressed(int)));
}

Calc::~Calc()
{
    delete ui;
}
