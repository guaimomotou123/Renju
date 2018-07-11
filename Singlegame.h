#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "Board.h"
#include "Eveluation.h"

class SingleGame : public Board
{
    Q_OBJECT
public:
    SingleGame()
    {
    }
    virtual void click(int row, int col);
    Step* getBestMove();
    void getAllPossibleMove(QVector<Step*>& steps);
    void fakeMove(Step* step);
    void unfakeMove(Step* step);
    int calcScore();
    void saveStep(int row, int col, QVector<Step*>& steps);
    int m_nUserStoneColor = BLACK;                  //用户棋子的颜色
    int getMinScore(int level, int curMaxScore);
    int getMaxScore(int level, int curMinScore);
    int m_nSelectedPly = 3; //记录用户选择的搜索的深度

    /*判断是否赢了*/
    bool isWin(int row, int col);
    bool JudgeVertical(int row, int col);
    bool JudgeHorizon(int row, int col);
    bool JudgeLeft(int row, int col);
    bool JudgeRight(int row, int col);
public slots:
    void computerMove();
protected:
    Eveluation *m_pEvel = new Eveluation;    //估分指针
};

#endif // SINGLEGAME_H
