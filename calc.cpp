#include "calc.h"
#include "ui_calc.h"
#include <QSignalMapper>
#include <QRegularExpression>

bool Calc::isOperand(QChar c)
{
        return c == '/' || c == '*' || c == '-' ||
           c == '+' || c == '%';
}

QString Calc::get_answer() {
    QString equation = this->equation;
    if(equation == "") return "0"; /* if equation is blank, then return 0 */
    if(isOperand(equation.at(0)) || isOperand(equation.at(equation.size()-1))) return "ERROR"; /* if first or last char
                                                                                                 is an operator return error */
    bool equationIsNumber = true;
    for(int i=0; i < equation.size(); ++i)
    {
        // if period is at the end of the equation, or operand/period is to the right of period, then return error
        if(equation.at(i) == '.' && ((i+1) == equation.size() || isOperand(equation.at(i+1)) || equation.at(i+1) == '.')) return "ERROR";
        // if equation has an operand, then the equation is not a number
        if(isOperand(equation.at(i))) equationIsNumber = false;
    }
    if(equationIsNumber) return QString::number(equation.toDouble()); // return converted double if number
    QVector<double> nums;
    QVector<QChar> operands;
    for(int i=0; i < equation.size(); ++i)
    {
        if(isOperand(equation.at(i)))
        {
            if(equation.at(i) == equation.at(i+1)) return "ERROR"; //returns error if operand to the right of operand
            //targets first occurance of operation because num size is 0(no number added yet)
            if(nums.size() == 0)
            {
                //add first number
                nums.push_back(equation.left(i).toDouble());
            }
            //matches a valid number (regex)
            QString right = equation.right(equation.size() - i);
            QRegularExpression re("\\d*(\\d(?=[/*\\-+%])|$|\\.)\\d*");
            QRegularExpressionMatch match = re.match(right);

            //adds the number and operand to vector
            nums.push_back(match.captured().toDouble());
            operands.push_back(equation.at(i));
        }
    }
    /*
     * Order of Operations:
     * first targets the * | / | % operands
     * numbers that operand modifies has operation done with the
     * next number in the vector, and number is set to 0
     *
     * Ex:
     * Before Operation: {1,2,3,4}
     *                    {+,*,-}
     * After Operation:  {1,0,6,4}
     *                    {+,*,-}
     *
     * At the end, once * | / | % is done, another pass is done to add and subtract numbers
     * from left to right, making sure not to interfere with the * | / | % operands
     *
     * First Operation: {1,0,6,4}
     *                    {+,*,-}
     * Next Operation: {1,0,7,4} <<- index 2 is added with 0 because * operand occupies the first index
     *                  {+,*,-} <<- plus operation is done
     * Last Operation: {1,0,7,3} <<- index 3 is set to index 2 - index 3
     *                  {+,*,-} <<- subtract operation is done
    */
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
    //return last number in vector which should be answer(?)
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
