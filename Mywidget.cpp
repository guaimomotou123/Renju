#include "Mywidget.h"
#include "ui_Mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    setFixedSize(300,200);
    //b.show();
    /*处理选择窗口隐藏，棋盘窗口显示的信号*/
    connect(ui->OKButton, &QPushButton::released, this, &MyWidget::Changewin);
    /*处理选择窗口显示，棋盘窗口隐藏的信号*/
    connect(&b, &SingleGame::from_board_to_mywidgetSignal, this, &MyWidget::dealBoard);
    ui->DeepthBox->setRange(1,5);   //设置搜索深度范围
}

void MyWidget::Changewin()
{
    this->hide();   //选择窗口隐藏
    b.show();       //棋盘窗口显示
}

void MyWidget::dealBoard()
{
    this->show();   //选择窗口显示
    b.hide();       //棋盘窗口隐藏
}

MyWidget::~MyWidget()
{
    delete ui;
}


void MyWidget::on_BlackButton_pressed()
{
    m_nStoneColor = BLACK;
}

void MyWidget::on_WhiteButton_pressed()
{
    m_nStoneColor = WHITE;
}

void MyWidget::on_DeepthBox_valueChanged(int arg1)
{
    b.m_nSelectedPly = arg1;
}
