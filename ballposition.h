
#ifndef __BALLPOSITION_H_ 
#define __BALLPOSITION_H_


#include <QPoint>
#include "animatedmove.h"

class BallPosition{
public:
    BallPosition();
    ~BallPosition();

    int m_nLogicalRow;
    int m_nLogicalCol;

    void setCurrentPosition(QPoint position);
    void moveTo(QPoint to);
    QPoint currentPosition() const;

    void updatePosition();

    const AnimatedMove animation(){
        return m_cAnimation;
    };
    
    void setAnimationTimestep(int ms){
        m_cAnimation.setTimestep(ms);
    };
    int animationTimestep() const { return m_cAnimation.timestep(); };
    

private:
    QPoint m_cCurrentPosition;
    AnimatedMove m_cAnimation;
};


#endif
