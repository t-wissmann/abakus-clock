
#ifndef __TW_ANIMATEDMOVE_H_
#define __TW_ANIMATEDMOVE_H_

#include <QPoint>
#include <QTime>

class AnimatedMove{
    
public:
    AnimatedMove();
    AnimatedMove(QPoint from, QPoint to);
    ~AnimatedMove();
    
    void start();
    void start(QPoint from, QPoint to);
    
    QPoint target() const { return m_cTo; };
    
    bool animationFinished() const;
    
    static int defaultValueAt(int from, int to, float position);
    static int linearValueAt(int from, int to, float position);
    static int sinusValueAt(int from, int to, float position);
    
    QPoint currentPosition();
    void setTimestep(int ms);
    int timestep() const { return m_nTimeStep; };
    
private:
    QPoint m_cFrom;
    QPoint m_cTo;
    
    bool   m_bAnimationFinished;
    int    m_nTimeStep; // in ms for complete animation
    QTime  m_cStartTime;
    float  m_nLastPosition;
};


#endif
