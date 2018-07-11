#include "Eveluation.h"
#include "Stone.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

/* 位置重要性价值表，此表从中间向外，越往外价值越低*/
int PosValue[GRID_NUM][GRID_NUM]=
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
int count = 0;  //全局变量，用以统计估值函数的执行遍数

Eveluation::Eveluation()
{

}

Eveluation::~Eveluation()
{
}

/*
估值函数
position是要估值的棋盘
bIsWhiteTurn是轮到谁走棋的标志，TRUE是白，FALSE是黑
*/
int Eveluation::CalScore(int position[][GRID_NUM], bool bIsWhiteTurn)
{
    int i, j, k;
    int nStoneType;
    count++;
    /* 清空棋型分析结果*/
    for(int row=0; row<GRID_NUM; row++)
    {
        for(int col=0; col<GRID_NUM;col++)
        {
            for(int k=0; k<4; k++)
            {
                TypeRecord[row][col][k] = 0;
            }
        }
    }
    for(int row=0; row<3; row++)
    {
        for(int col=0; col<20;col++)
        {
            TypeCount[row][col] = 0;
        }
    }
    for(int i=0; i<30; i++)
    {
        m_LineRecord[i] = 0;
    }
    //memset(TypeRecord, 0, 15*15*4*sizeof(int));
    //memset(TypeCount, 0, 30*sizeof(int));
    /* 对棋盘上所有棋子在 源 个方向上进行分析*/
    for(i=0; i<GRID_NUM; i++)
    {
        for(j=0; j<GRID_NUM; j++)
        {
            if(position[i][j] != 0)
            {
                /*如果水平方向上没有分析过*/
                if(TypeRecord[i][j][0] == TOBEANALSIS)
                {
                    AnalysisHorizon(position, i, j);
                }
                /*如果垂直方向上没有分析过*/
                if(TypeRecord[i][j][1] == TOBEANALSIS)
                {
                    AnalysisVertical(position, i, j);
                }
                /*如果左斜方向上没有分析过*/
                if(TypeRecord[i][j][2] == TOBEANALSIS)
                {
                    AnalysisLeft(position, i, j);
                }
                /*如果右斜方向上没有分析过*/
                if(TypeRecord[i][j][3] == TOBEANALSIS)
                {
                    AnalysisRight(position, i, j);
                }
            }
        }
    }
    /*对分析结果进行统计，得到每种棋型的数量*/
    for(i=0; i<GRID_NUM; i++)
    {
        for(j=0; j<GRID_NUM; j++)
        {
            for(k=0; k<4; k++)
            {
                nStoneType = position[i][j];
                if(nStoneType != 0)
                {
                    switch(TypeRecord[i][j][k])
                    {
                    case FIVE:  //五连
                        TypeCount[nStoneType][FIVE]++;
                        //qDebug() << i << j <<endl;
                        break;
                    case FOUR:  //活四
                        TypeCount[nStoneType][FOUR]++;
                        break;
                    case SFOUR: // 冲四
                        TypeCount[nStoneType][SFOUR]++;
                        break;
                    case THREE: //活三
                        TypeCount[nStoneType][THREE]++;
                        break;
                    case STHREE://眠三
                        TypeCount[nStoneType][STHREE]++;
                        break;
                    case TWO:   // 活二
                        TypeCount[nStoneType][TWO]++;
                        break;
                    case STWO:  //眠二
                        TypeCount[nStoneType][STWO]++;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    /* 如果已五连，返回极值*/
    if(bIsWhiteTurn)
    {
        if(TypeCount[BLACK][FIVE])
        {
            return -9999;
        }
        if(TypeCount[WHITE][FIVE])
        {
            return 9999;
        }
    }
    else
    {
        if(TypeCount[BLACK][FIVE])
        {
            return -9999;
        }
        if(TypeCount[WHITE][FIVE])
        {
            return 9999;
        }
    }
    /*两个冲四等于一个活四*/
    if(TypeCount[WHITE][SFOUR] > 1)
    {
        TypeCount[WHITE][FOUR]++;
        TypeCount[WHITE][SFOUR] -= 2;
    }
    if(TypeCount[BLACK][SFOUR] > 1)
    {
        TypeCount[BLACK][FOUR]++;
        TypeCount[BLACK][SFOUR] -= 2;
    }
    int WValue = 0, BValue = 0;
    if(bIsWhiteTurn)//轮到白棋走
    {
        if(TypeCount[WHITE][FOUR])//活四，白胜返回极值
        {
            return -9990;
        }
        if(TypeCount[WHITE][SFOUR])//冲四，白胜返回极值
        {
            return -9980;
        }
        if(TypeCount[BLACK][FOUR])//白无冲四活四，而黑有活四，黑胜返回极值
        {
            return 9970;
        }
        /*而黑有冲四和活三，黑胜返回极值*/
        if(TypeCount[BLACK][SFOUR] && TypeCount[BLACK][THREE])
        {
            return 9960;
        }
        /*白有活三而黑没有四，白胜返回极值*/
        if(TypeCount[WHITE][THREE] && TypeCount[BLACK][SFOUR] == 0)
        {
            return -9950;
        }
        /*黑的活三多于一个，而白无四和三，黑胜返回极值*/
        if(TypeCount[BLACK][THREE] > 1 && TypeCount[WHITE][SFOUR] == 0 &&
           TypeCount[WHITE][THREE] == 0 && TypeCount[WHITE][STHREE] == 0)
        {
            return 9940;
        }
        /*白活三多于一个，白棋价值加2000*/
        if(TypeCount[WHITE][THREE] > 1)
        {
            WValue += 500;
        }
        else
        {
            if(TypeCount[WHITE][THREE])
            {
                WValue += 100;
            }
        }
        /*黑的活三多于一个，黑棋价值加500*/
        if(TypeCount[BLACK][THREE] > 1)
        {
            BValue += 2000;
        }
        else
        {
            if(TypeCount[BLACK][THREE])
            {
                BValue += 200;
            }
        }
        /* 每个眠三加10*/
        if(TypeCount[WHITE][STHREE])
        {
            WValue += TypeCount[WHITE][STHREE] * 10;
        }
        if(TypeCount[BLACK][STHREE])
        {
            BValue += TypeCount[BLACK][STHREE] * 10;
        }
        /*每个活二加4*/
        if(TypeCount[WHITE][TWO])
        {
            WValue += TypeCount[WHITE][TWO] * 4;
        }
        if(TypeCount[BLACK][TWO])
        {
            BValue += TypeCount[BLACK][TWO] * 4;
        }
        /*每个眠二加1*/
        if(TypeCount[WHITE][STWO])
        {
            WValue += TypeCount[WHITE][STWO] * 4;
        }
        if(TypeCount[BLACK][STWO])
        {
            BValue += TypeCount[BLACK][STWO] * 4;
        }
    }
    else
    {// 轮到黑棋走
        if(TypeCount[BLACK][FOUR])//活四，黑胜返回极值
        {
            return -9990;
        }
        if(TypeCount[BLACK][SFOUR])//冲四，黑胜返回极值
        {
            return -9980;
        }
        if(TypeCount[WHITE][FOUR])//活四，白胜返回极值
        {
            return 9970;
        }
        /*冲四并活三，白胜返回极值*/
        if(TypeCount[WHITE][SFOUR] && TypeCount[WHITE][THREE])
        {
            return 9960;
        }
        /*黑活三，白无四。黑胜返回极值*/
        if(TypeCount[BLACK][THREE] && TypeCount[BLACK][SFOUR] == 0)
        {
            return -9950;
        }
        /*白的活三多于一个，而黑无四和三，白胜返回极值*/
        if(TypeCount[WHITE][THREE] > 1 && TypeCount[BLACK][SFOUR] == 0 &&
           TypeCount[BLACK][THREE] == 0 && TypeCount[BLACK][STHREE] == 0)
        {
            return 9940;
        }
        /*黑的活三多于一个，黑棋价值加 2000*/
        if(TypeCount[BLACK][THREE] > 1)
        {
            BValue += 500;
        }
        else
        {
            if(TypeCount[BLACK][THREE])
            {
                BValue += 100;
            }
        }
        /*白的活三多于一个，白棋价值加500*/
        if(TypeCount[WHITE][THREE] > 1)
        {
            WValue += 2000;
        }
        else
        {
            if(TypeCount[WHITE][THREE])
            {
                WValue += 200;
            }
        }
        /* 每个眠三加10*/
        if(TypeCount[WHITE][STHREE])
        {
            WValue += TypeCount[WHITE][STHREE] * 10;
        }
        if(TypeCount[BLACK][STHREE])
        {
            BValue += TypeCount[BLACK][STHREE] * 10;
        }
        /*每个活二加4*/
        if(TypeCount[WHITE][TWO])
        {
            WValue += TypeCount[WHITE][TWO] * 4;
        }
        if(TypeCount[BLACK][TWO])
        {
            BValue += TypeCount[BLACK][TWO] * 4;
        }
        /*每个眠二加1*/
        if(TypeCount[WHITE][STWO])
        {
            WValue += TypeCount[WHITE][STWO] * 4;
        }
        if(TypeCount[BLACK][STWO])
        {
            BValue += TypeCount[BLACK][STWO] * 4;
        }
    }
    /*加上所有棋子的位置价值*/
    for(i=0; i<GRID_NUM; i++)
    {
        for(j=0; j<GRID_NUM; j++)
        {
            nStoneType = position[i][j];
            if(nStoneType == BLACK)
            {
                BValue += PosValue[i][j];
            }
            else if(nStoneType == WHITE)
            {
                WValue += PosValue[i][j];
            }
        }
    }
    if(bIsWhiteTurn)
    {
        return BValue - WValue;
    }
    else
    {
        return WValue - BValue;
    }
}

/*分析棋盘上某点在水平方向上的棋型*/
int Eveluation::AnalysisHorizon(int position[][GRID_NUM], int i, int j)
{
    int tempArray[GRID_NUM];
    /*将水平方向上的棋子转入一维数组*/
    for(int k=0; k<GRID_NUM; k++)
    {
        tempArray[k] = position[i][k];
    }
    /*调用直线分析函数分析*/
    AnalysisLine(tempArray, GRID_NUM, j);
    /*拾取分析结果*/
    for(int s=0; s<15; s++)
    {
        if(m_LineRecord[s] != TOBEANALSIS)
        {
            TypeRecord[i][s][0] = m_LineRecord[s];
        }
    }
    return TypeRecord[i][j][0];
}

/*分析棋盘上某点在垂直方向上的棋型*/
int Eveluation::AnalysisVertical(int position[][GRID_NUM], int i, int j)
{
    int tempArray[GRID_NUM];
    /*将垂直方向上的棋子转入一维数组*/
    for(int k=0; k<GRID_NUM; k++)
    {
        tempArray[k] = position[k][j];
    }
    /*调用直线分析函数分析*/
    AnalysisLine(tempArray, GRID_NUM, i);
    /*拾取分析结果*/
    for(int s=0; s<GRID_NUM; s++)
    {
        if(m_LineRecord[s] != TOBEANALSIS)
        {
            TypeRecord[s][j][1] = m_LineRecord[s];
        }
    }
    return TypeRecord[i][j][1];
}

/*分析棋盘上某点在左斜方向上的棋型*/
int Eveluation::AnalysisLeft(int position[][GRID_NUM], int i, int j)
{
    int tempArray[GRID_NUM];
    int x, y;
    if(i < j)
    {
        y = 0;
        x = j - i;
    }
    else
    {
        x = 0;
        y = i - j;
    }
    /*将斜方向上的棋子转入一维数组*/
    int k;
    for(k=0; k<GRID_NUM; k++)
    {
        if(x+k>14 || y+k>14)
        {
            break;
        }
        tempArray[k] = position[y+k][x+k];
    }
    /*调用直线分析函数分析*/
    AnalysisLine(tempArray, k, j-x);
    /* 拾取分析结果*/
    for(int s=0; s<k; s++)
    {
        if(m_LineRecord[s] != TOBEANALSIS)
        {
            TypeRecord[y+s][x+s][2] = m_LineRecord[s];
        }
    }
    return TypeRecord[i][j][2];
}

/*分析棋盘上某点在右斜方向上的棋型*/
int Eveluation::AnalysisRight(int position[][GRID_NUM], int i, int j)
{
    int tempArray[GRID_NUM];
    int x, y, realnum;
    if(14-i < j)
    {
        y = 14;
        x = j -14 + i;
        realnum = 14 - i;
    }
    else
    {
        x = 0;
        y = i + j;
        realnum = j;
    }
    /*将斜方向上的棋子转入一维数组*/
    int k;
    for(k=0; k<GRID_NUM; k++)
    {
        if(x+k>14 || y-k<0)
        {
            break;
        }
        tempArray[k] = position[y-k][x+k];
    }
    /*调用直线分析函数分析*/
    AnalysisLine(tempArray, k, j-x);
    /* 拾取分析结果*/
    for(int s=0; s<k; s++)
    {
        if(m_LineRecord[s] != TOBEANALSIS)
        {
            TypeRecord[y-s][x+s][3] = m_LineRecord[s];
        }
    }
    return TypeRecord[i][j][3];
}

/*
直线分析函数
此函数分析给定一维数组当中有多少五、四、三、二等棋型
*/
int Eveluation::AnalysisLine(int *position, int GridNum, int StonePos)
{
    int StoneType;
    int AnalyLine[30];
    int nAnalyPos;
    int LeftEdge, RightEdge;
    int LeftRange, RightRange;
    if(GridNum < 5)
    {
        for(int i=0; i<GridNum; i++)
        {
            m_LineRecord[i] = 0;
        }
        //memset(m_LineRecord, ANALSISEN, GridNum*sizeof(int));
        return 0;
    }
    nAnalyPos = StonePos;
    for(int i=0; i<GRID_NUM; i++)
    {
        m_LineRecord[i] = 0;
    }
    //memset(m_LineRecord, TOBEANALSIS, GRID_NUM*sizeof(int));
    for(int i=0; i<30; i++)
    {
        AnalyLine[i] = 0;
    }
    /*将传入数组装入AnalyLine*/
    for(int i=0; i<GridNum; i++)
    {
        AnalyLine[i] = position[i];
    }
    GridNum--;
    StoneType = AnalyLine[nAnalyPos];
    LeftEdge = nAnalyPos;
    RightEdge = nAnalyPos;
    /*算连续棋子左边界*/
    while(LeftEdge > 0)
    {
        if(AnalyLine[LeftEdge-1] != StonePos)
        {
            break;
        }
        LeftEdge--;
    }
    /*算连续棋子右边界*/
    while(RightEdge < GridNum)
    {
        if(AnalyLine[RightEdge+1] != StoneType)
        {
            break;
        }
        RightEdge++;
    }
    LeftRange = LeftEdge;
    RightRange = RightEdge;
    /*下面两个循环算出棋子可下的范围*/
    while(LeftRange > 0)
    {
        if(AnalyLine[LeftRange-1] == (StoneType%2+1))
        {
            break;
        }
        LeftRange--;
    }
    while(RightRange < GridNum)
    {
        if(AnalyLine[RightRange+1] == (StoneType%2+1))
        {
            break;
        }
        RightRange++;
    }
    /*如果此范围小于4则分析没有意义*/
    if(RightRange - LeftRange < 4)
    {
        for(int k=LeftRange; k<=RightRange; k++)
        {
            m_LineRecord[k] = ANALSISEN;
        }
        return 1;
    }
    /*将连续区域设为分析过的，防止重复分析此一区域*/
    for(int k=LeftEdge; k<=RightEdge; k++)
    {
        m_LineRecord[k] = ANALSISEN;
    }
    if(RightEdge-LeftEdge > 3)
    {//如待分析棋子棋型为五连
        m_LineRecord[nAnalyPos] = FIVE;
        //qDebug() << RightEdge-LeftEdge << "FIVE";
        return 1;
    }
    if(RightEdge-LeftEdge == 3)//有四颗相同颜色的棋
    {
        bool Leftfour = false;
        if(LeftEdge > 0)
        {
            if(AnalyLine[LeftEdge-1] == 0)
            {
                Leftfour = true;//左边有气
            }
        }
        if(RightEdge < GridNum)
        {//右边未到边界
            if(AnalyLine[RightEdge+1] == 0)
            {//右边有气
                if(Leftfour == true)
                {// 如左边有气
                    m_LineRecord[nAnalyPos] = FOUR; //活四
                }
                else
                {
                    m_LineRecord[nAnalyPos] = SFOUR; //冲四
                }
            }
            else
            {
                if(Leftfour == true)
                {//如左边有气
                    m_LineRecord[nAnalyPos] = SFOUR; //冲四
                }
            }
        }
        else
        {
            if(Leftfour = true)
            {//如左边有气
                m_LineRecord[nAnalyPos] = SFOUR; //冲四
            }
        }
        return 1;
    }
    if(RightEdge-LeftEdge == 2)
    {//如待分析棋子棋型为三连
        bool LeftThree = false;
        if(LeftEdge > 1)
        {
            if(AnalyLine[LeftEdge-1] == 0)
            {//左边有气
                if(LeftEdge>1 && (AnalyLine[LeftEdge-2] == AnalyLine[LeftEdge]))
                {//左边隔一空白有己方棋子
                    m_LineRecord[LeftEdge] = SFOUR; //冲四
                    m_LineRecord[LeftEdge-2] = ANALSISEN;
                }
                else
                {
                    LeftThree = true;
                }
            }
        }
        if(RightEdge < GridNum)
        {
            if(AnalyLine[RightEdge+1] == 0)
            {//右边有气
                if(RightEdge<(GridNum-1) && (AnalyLine[RightEdge+2] == AnalyLine[RightEdge]))
                {//右边隔1个己方棋子
                    m_LineRecord[RightEdge] = SFOUR; //冲四
                    m_LineRecord[RightEdge+2] = ANALSISEN;
                }
                else
                {
                    if(LeftThree == true)
                    {//如左边有气
                        m_LineRecord[RightEdge] = THREE; //活三
                    }
                    else
                    {
                        m_LineRecord[RightEdge] = STHREE; //冲三
                    }
                }
            }
            else
            {
                if(m_LineRecord[LeftEdge] == SFOUR)
                {//如左冲四
                    return m_LineRecord[LeftEdge];
                }
                if(LeftThree == true)
                {// 如左边有气
                    m_LineRecord[nAnalyPos] = STHREE;// 眠三
                }
            }
        }
        else
        {
            if(m_LineRecord[LeftEdge] == SFOUR)
            {//如左冲四
                return m_LineRecord[LeftEdge];
            }
            if(LeftThree == true)
            {//如左边有气
                m_LineRecord[nAnalyPos] = STHREE;
            }
        }
        return 1;
    }
    if(RightEdge - LeftEdge == 1)
    {//如待分析棋子棋型为二连
        bool Lefttwo = false;
        bool leftthree = false;
        if(LeftEdge > 2)
        {
            if(AnalyLine[LeftEdge-1] == 0)
            {// 左边有气
                if(LeftEdge-1>1 && AnalyLine[LeftEdge-2]==AnalyLine[LeftEdge])
                {
                    if(AnalyLine[LeftEdge-3] == AnalyLine[LeftEdge])
                    {//左边隔2个己方棋子
                        m_LineRecord[LeftEdge-3] = ANALSISEN;
                        m_LineRecord[LeftEdge-2] = ANALSISEN;
                        m_LineRecord[LeftEdge] = SFOUR;
                    }
                    else
                    {
                        if(AnalyLine[LeftEdge-3] == 0)
                        {//左边隔1个己方棋子
                            m_LineRecord[LeftEdge-2] = ANALSISEN;
                            m_LineRecord[LeftEdge] = STHREE;
                        }
                    }
                }
                else
                {
                    Lefttwo = true;
                }
            }
        }
        if(RightEdge < GridNum-2)
        {
            if(AnalyLine[RightEdge+1] == 0)
            {//右边有气
                if(RightEdge+1 < GridNum-1 && AnalyLine[RightEdge+2]==AnalyLine[RightEdge])
                {
                    if(AnalyLine[RightEdge+3] == AnalyLine[RightEdge])
                    {//右边隔两个己方棋子
                        m_LineRecord[RightEdge+3] = ANALSISEN;
                        m_LineRecord[RightEdge+2] = ANALSISEN;
                        m_LineRecord[RightEdge] = SFOUR;
                    }
                    else
                    {
                        if(AnalyLine[RightEdge+3] == 0)
                        {//右边隔1个己方棋子
                            m_LineRecord[RightEdge+2] = ANALSISEN;
                            m_LineRecord[RightEdge] = STHREE;
                        }
                    }
                }
                else
                {
                    if(m_LineRecord[LeftEdge] == SFOUR)
                    {
                        return m_LineRecord[LeftEdge];
                    }
                    if(m_LineRecord[LeftEdge] == STHREE)
                    {
                        return m_LineRecord[LeftEdge];
                    }
                    if(Lefttwo == true)
                    {
                        m_LineRecord[nAnalyPos] = TWO;
                    }
                    else
                    {
                        m_LineRecord[nAnalyPos] = STWO;
                    }
                }
            }
            else
            {
                if(m_LineRecord[LeftEdge] == SFOUR)
                {
                    return m_LineRecord[LeftEdge];
                }
                if(Lefttwo == true)
                {
                    m_LineRecord[nAnalyPos] = STWO;
                }
            }
        }
        return 1;
    }
    return 0;
}
