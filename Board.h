#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPushButton>
#include <QPoint>
#include <QPointer>
#include "Stone.h"
#include "Step.h"

//typedef struct _movestone
//{
//    int nQiZiID;
//    STONEPOS ptMovePoint;
//}MOVESTONE;

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    void sendSlot();
    void InvertRenjiBoard();//重绘棋盘

    int m_RenjiBoard[GRID_NUM][GRID_NUM];   //棋盘数组，用于显示棋盘
    bool _bBlackTurn = 1;   //黑棋先走

    int _r = 20;//棋子的半径

    void paintEvent(QPaintEvent *);//绘制棋盘
    void mouseReleaseEvent(QMouseEvent *);

    QPoint center(int row, int col);//返回棋盘行列对应的像素坐标
    bool getRowCol(QPoint pt, int &row, int &col);//得到此时对应棋盘的行和列
    void click(QPoint pt);
    virtual void click(int row, int col);
protected:
    //virtual void DoDataExchange()


signals:
    void from_board_to_mywidgetSignal();

public slots:
private:
    QPushButton newGame;

    int m_nUserStoneColor = BLACK;                  //用户棋子的颜色

};

#endif // BOARD_H
