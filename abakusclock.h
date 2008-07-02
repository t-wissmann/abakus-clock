
#ifndef __ABAKUSCLOCK_H_
#define __ABAKUSCLOCK_H_

#include <QWidget>
#include <QList>

#include "ballposition.h"


class QTimer;
class QPaintEvent;
class QResizeEvent;


class AbakusClock : public QWidget{
    Q_OBJECT
public:
    AbakusClock();
    virtual ~AbakusClock();
    
    void recomputeLogicalBallPositions(bool animated = TRUE);
    void setLogicalBallPositionsInCol(BallPosition* col,
                            int colheight, int value, bool animated = TRUE);
    void computeRealFromLogicalBallPosition();
    int  getYByRowNumber(int row);
    void setTime(char hours, char mins);
    void setAnimationTimestep(int ms);
    int animationTimestep() const;
    
    void setAddingMinTimestep(int s);
    int  addingMinTimestep() const;
    
public slots:
    void moveBallPositions();
    void addMinute();
protected:
    virtual void resizeEvent(QResizeEvent* e);
    virtual void paintEvent(QPaintEvent* e);
    virtual void paintBall(QPainter& painter, int centerX, int centerY, int height);
    virtual void printAxis(QPainter& painter, int x, int y, int height, int width = 4);
private:
    void initGraphicalMembers();
    
    QTimer*      m_tmrAddMinute;
    
    BallPosition m_rgBalls[20];
    char         m_nHours;
    char         m_nMins;
    float        m_nBallDiameter;
    int          m_nMargin;  // margin in px
    int          m_nBallspacing;
    int          m_nMiddlebarheight;
    // every m_nAddingMinTimestep milli secondes will be added 1 minute to current time
    // if m_nAddingMinTimestep <= 0, nothing will happen
    int          m_nAddingMinTimestep; 
};

#endif
