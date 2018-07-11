#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

Board::Board(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle("人机大战");
    this->_r = 20;
    setFixedSize(_r*2*19+1, _r*2*16);
    newGame.setParent(this);
    newGame.move(_r*2*16, _r*2*8);
    newGame.setFixedSize(_r*4, _r*1.5);
    newGame.setText("New Game");
    connect(&newGame, &QPushButton::clicked, this, &Board::InvertRenjiBoard);
    connect(&newGame, &QPushButton::clicked, this, &Board::sendSlot);
    for(int row=0; row<GRID_NUM; row++)
    {
        for(int col=0; col<GRID_NUM;col++)
        {
            m_RenjiBoard[row][col] = 0; //初始化棋盘
        }
    }
    //memset(m_RenjiBoard, 0, GRID_COUNT * sizeof(int));    //初始化棋盘

}

void Board::InvertRenjiBoard()
{//重绘棋盘
    for(int row=0; row<GRID_NUM; row++)
    {
        for(int col=0; col<GRID_NUM;col++)
        {
            m_RenjiBoard[row][col] = 0;
        }
    }
    //memset(m_RenjiBoard, 0, GRID_COUNT * sizeof(int));
    update();
}

void Board::sendSlot()
{
    emit from_board_to_mywidgetSignal();    //将棋盘收到的信号发送给选择窗口处理
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int d = 2 * _r;
    //_r = d/2;
    for(int i=1; i<16; i++)
    {
        painter.drawLine(QPoint(d, i*d), QPoint(15*d, i*d));
        painter.drawLine(QPoint(i*d, d), QPoint(i*d, 15*d));
    }
    painter.save();
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    for(int row=0; row<GRID_NUM; row++)
    {
        for(int col=0; col<GRID_NUM; col++)
        {
            if(m_RenjiBoard[row][col] == 1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(center(row, col), _r-2, _r-2);
            }
            else if(m_RenjiBoard[row][col] == 2)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(center(row, col), _r-2, _r-2);
            }

        }
    }
    painter.restore();
    update();
}

QPoint Board::center(int row, int col)
{//返回棋盘行列对应的像素坐标
    QPoint ret;
    ret.rx() = (col+1)* _r*2;
    ret.ry() = (row+1)* _r*2;
    return ret;
}

bool Board::getRowCol(QPoint pt, int &row, int &col)
{//得到此时对应棋盘的行和列
    for(row=0; row<16; row++)
    {
        for(col=0; col<16; col++)
        {
            QPoint distance = center(row, col) - pt;
            if(distance.x() * distance.x() + distance.y() * distance.y() < _r * _r)
                return true;
        }
    }
    return false;
}

void Board::click(int row, int col)
{
    qDebug() << "父类";
    if(m_RenjiBoard[row][col] == 0)
    {//防止重复落子
        m_RenjiBoard[row][col] = BLACK;
        _bBlackTurn = !_bBlackTurn;
    }
}

void Board::click(QPoint pt)
{

    int row = 0;
    int col = 0;
    bool bRet = getRowCol(pt, row, col);
    if(bRet == false)
    {//点到棋盘外
        return;
    }
    click(row, col);
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
    {
        return;
    }
    click(ev->pos());
}

