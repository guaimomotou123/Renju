#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
public:
    Step(QObject *parent = 0);
    ~Step();
    int _rowTo;
    int _colTo;
};

#endif // STEP_H
