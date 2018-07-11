#ifndef EVELUATION_H
#define EVELUATION_H
#include "Stone.h"

/* 这组宏定义了用以代表几种棋型的数字*/
#define STWO 1  //眠二
#define STHREE 2//眠三
#define SFOUR 3 //冲四
#define TWO 4   //活二
#define THREE 5 //活三
#define FOUR 6  //活四
#define FIVE 7  //五连
#define NOTYPE 11 //未定义
#define ANALSISEN 255   //已分析过的
#define TOBEANALSIS 0   //已分析过的
extern int PosValue[15][15];    //棋子位置价值表

class Eveluation
{
public:
    Eveluation();
    virtual ~Eveluation();
    /*估值函数 ，对传入的棋盘打分.bIsWhiteTurn标明轮到谁走棋*/
    int CalScore(int position[][GRID_NUM],bool bIsWhiteTurn);
protected:
    /* 分析水平方向上某点及其周边的棋型*/
    int AnalysisHorizon(int position[][GRID_NUM], int i, int j);
    /*分析垂直方向上某点及其周边的棋型*/
    int AnalysisVertical(int position[][GRID_NUM], int i, int j);
    /*分析左斜45度方向上某点及其周边的棋型*/
    int AnalysisLeft(int position[][GRID_NUM], int i, int j);
    /*分析右斜45度方向上某点及其周边的棋型*/
    int AnalysisRight(int position[][GRID_NUM], int i, int j);
    /*分析给定行上某点及其周边的棋型*/
    int AnalysisLine(int *position, int GridNum, int StonePos);
    /*存放AnalysisLine分析结果的数组*/
    int m_LineRecord[30];
    /*
    存放全部分析结果的数组
    有三个维度，用于存放水平、垂直、左斜、右斜 源 个方向上所有棋型
    分析结果
    */
    int TypeRecord[GRID_NUM][GRID_NUM][4];
    /*存放统记过的分析结果的数组*/
    int TypeCount[3][20];
};

#endif // EVELUATION_H
