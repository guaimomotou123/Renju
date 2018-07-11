#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "Singlegame.h"
#include "Stone.h"
#include <QPushButton>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
    void Changewin();
    void dealBoard();
    //int m_nSelectedPly = 3; //记录用户选择的搜索的深度
protected:

    int m_nStoneColor = BLACK;  //记录用户选择的棋子的颜色,默认棋子用户的棋子为黑色

private slots:
    void on_BlackButton_pressed();
    void on_WhiteButton_pressed();
    void on_DeepthBox_valueChanged(int arg1);

private:
    Ui::MyWidget *ui;
    SingleGame b;
};

#endif // MYWIDGET_H
