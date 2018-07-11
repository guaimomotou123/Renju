#include "Singlegame.h"
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>

void SingleGame::click(int row, int col)
{
    //qDebug() << "子类";
    if(m_RenjiBoard[row][col] == 0 && _bBlackTurn)
    {//防止重复落子
        Board::click(row, col);
        if(isWin(row, col))
        {
            QMessageBox::information(this, "Win", "恭喜你赢了！", QMessageBox::Ok);
        }
    }
    if(!_bBlackTurn)
    {
        QTimer::singleShot(10, this, SLOT(computerMove()));
        //computerMove();
    }
}


void SingleGame::saveStep(int row, int col, QVector<Step*>& steps)
{
    Step* step = new Step;
    step->_colTo = col;
    step->_rowTo = row;
    steps.append(step);
}

void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
    for(int row=0; row<GRID_NUM; row++)
    {
        for(int col=0; col<GRID_NUM; col++)
        {
            if(m_RenjiBoard[row][col] != 0)
            {
                continue;
            }
            saveStep(row, col, steps);

        }
    }
}

void SingleGame::fakeMove(Step* step)
{
    if(_bBlackTurn)
    {
        m_RenjiBoard[step->_rowTo][step->_colTo] = BLACK;
    }
    else
    {
        m_RenjiBoard[step->_rowTo][step->_colTo] = WHITE;
    }
    _bBlackTurn = !_bBlackTurn;
}
void SingleGame::unfakeMove(Step* step)
{
    m_RenjiBoard[step->_rowTo][step->_colTo] = 0;
    _bBlackTurn = !_bBlackTurn;
}

/*评价局面分*/
int SingleGame::calcScore()
{
    int blackTotalScore = 0;
    int whiteTotalScore = 0;
    static int PosValue[GRID_NUM][GRID_NUM]=
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
        {0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
        {0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
        {0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
        {0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
        {0,1,2,3,4,5,6,7,6,5,4,3,2,1,0},
        {0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
        {0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
        {0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
        {0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
        {0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };

    /*白棋分 — 黑棋的分数*/
    for(int row=0; row<GRID_NUM; row++)
    {
        for(int col=0; col<GRID_NUM; col++)
        {
            if(m_RenjiBoard[row][col] == BLACK)
            {
                blackTotalScore += PosValue[row][col];
            }
            else if(m_RenjiBoard[row][col] == WHITE)
            {
                whiteTotalScore += PosValue[row][col];
            }
        }
    }
    return whiteTotalScore - blackTotalScore;
}

int SingleGame::getMaxScore(int level, int curMinScore)
{
    if(level == 0)
    {
        return m_pEvel->CalScore(m_RenjiBoard,!_bBlackTurn);
    }
    //看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);  //是白棋的possiblemove
    int maxScore = -100000;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();
        fakeMove(step);
        int score = getMinScore(level-1, maxScore);
        unfakeMove(step);
        delete step;

        if(score >= curMinScore)
        {//剪枝
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }
        if(score > maxScore)
        {
            maxScore = score;
            //ret = step;
        }
        //delete step;
    }
    return maxScore;
}

int SingleGame::getMinScore(int level, int curMaxScore)
{
    if(level == 0)
    {
        return m_pEvel->CalScore(m_RenjiBoard,!_bBlackTurn);
    }
    //看看有哪些步骤可以走
    QVector<Step*> steps;
    getAllPossibleMove(steps);  //是白棋的possiblemove
    int minScore = 100000;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();
        fakeMove(step);
        int score = getMaxScore(level-1, minScore);
        unfakeMove(step);
        delete step;
        if(score <= curMaxScore)
        {//剪枝
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }

        if(score < minScore)
        {
            minScore = score;
            //ret = step;
        }

    }
    return minScore;
}

Step* SingleGame::getBestMove()
{
    QVector<Step*> steps;
    getAllPossibleMove(steps);  //看看有哪些步骤可以走

    /*试着走一下*/
    int maxScore = -1000000;
    Step* ret = NULL;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();
        fakeMove(step);
        qDebug() << m_nSelectedPly << endl;
        int score = getMinScore(m_nSelectedPly-1, maxScore);
        //qDebug() << m_nSelectedPly << endl;
        unfakeMove(step);
        if(score > maxScore)
        {
            maxScore = score;
            if(ret)
            {
                delete ret;
            }
            ret = step;
        }
        else
        {
            delete step;
        }
    }
    return ret; //取最好的结果作为参考
}

void SingleGame::computerMove()
{
    Step* step = getBestMove();
    m_RenjiBoard[step->_rowTo][step->_colTo] = WHITE;
    if(isWin(step->_rowTo, step->_colTo))
    {
        QMessageBox::information(this, "战果", "你输了！", QMessageBox::Ok);
    }
    _bBlackTurn = !_bBlackTurn;
    delete step;
    update();
}

bool SingleGame::isWin(int row, int col)
{
     return JudgeVertical(row, col) || JudgeHorizon(row, col) || JudgeLeft(row, col) || JudgeRight(row ,col);
}

bool SingleGame::JudgeVertical(int row, int col)
{//判断垂直方向上是否有五连子
    int i;
    for (i = 0; i < 5; i++)
    {
        if(col - i >= 0 && col + 4 - i <= GRID_NUM &&
           m_RenjiBoard[row][col - i] == m_RenjiBoard[row][col + 1 - i] &&
           m_RenjiBoard[row][col - i] == m_RenjiBoard[row][col + 2 - i] &&
           m_RenjiBoard[row][col - i] == m_RenjiBoard[row][col + 3 - i] &&
           m_RenjiBoard[row][col - i] == m_RenjiBoard[row][col + 4 - i])
        {
            return true;
        }
    }
    return false;
}

bool SingleGame::JudgeHorizon(int row, int col)
{//判断水平方向上是否有五连子
    int i;
    for (i = 0; i < 5; i++)
    {
        if(row - i >= 0 && row + 4 - i <= GRID_NUM &&
           m_RenjiBoard[row - i][col] == m_RenjiBoard[row + 1 - i][col] &&
           m_RenjiBoard[row - i][col] == m_RenjiBoard[row + 2 - i][col] &&
           m_RenjiBoard[row - i][col] == m_RenjiBoard[row + 3 - i][col] &&
           m_RenjiBoard[row - i][col] == m_RenjiBoard[row + 4 - i][col])
           return true;
    }
    return false;
}

bool SingleGame::JudgeLeft(int row, int col)
{//判断左斜方向上是否有五连子
    int i;
    for (i = 0; i < 5; i++)
    {
        if(row - i >= 0 &&
           col - i >= 0 &&
           row + 4 - i <= 0xF &&
           col + 4 - i <= 0xF &&
           m_RenjiBoard[row - i][col - i] == m_RenjiBoard[row + 1 - i][col + 1 - i] &&
           m_RenjiBoard[row - i][col - i] == m_RenjiBoard[row + 2 - i][col + 2 - i] &&
           m_RenjiBoard[row - i][col - i] == m_RenjiBoard[row + 3 - i][col + 3 - i] &&
           m_RenjiBoard[row - i][col - i] == m_RenjiBoard[row + 4 - i][col + 4 - i])
           return true;
    }
    return false;
}

bool SingleGame::JudgeRight(int row, int col)
{//判断右斜方向上是否有五连子
    int i;
    for (i = 0; i < 5; i++)
    {
        if(row + i <= GRID_NUM && col - i >= 0 &&
           row - 4 + i >= 0 && col + 4 - i <= GRID_NUM &&
           m_RenjiBoard[row + i][col - i] == m_RenjiBoard[row - 1 + i][col + 1 - i] &&
           m_RenjiBoard[row + i][col - i] == m_RenjiBoard[row - 2 + i][col + 2 - i] &&
           m_RenjiBoard[row + i][col - i] == m_RenjiBoard[row - 3 + i][col + 3 - i] &&
           m_RenjiBoard[row + i][col - i] == m_RenjiBoard[row - 4 + i][col + 4 - i])
           return true;
    }
    return false;
}
