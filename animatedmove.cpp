
#include "animatedmove.h"

#include <math.h>


AnimatedMove::AnimatedMove()
{
    m_nTimeStep = 400;
    m_bAnimationFinished = TRUE;
    m_nLastPosition = 0;
}


AnimatedMove::AnimatedMove(QPoint from, QPoint to)
{
    m_bAnimationFinished = TRUE;
    m_nLastPosition = 0;
    m_nTimeStep = 400;
    m_cFrom = from;
    m_cTo = to;
}

AnimatedMove::~AnimatedMove()
{
    
}


bool AnimatedMove::animationFinished() const
{
    return m_bAnimationFinished;
}


int AnimatedMove::linearValueAt(int from, int to, float position)
{
    int value = (int)(from+ ((float)(to-from))*position );
    //qDebug("value=%d", value);
    return value;
}


int AnimatedMove::defaultValueAt(int from, int to, float position)
{
    return sinusValueAt(from, to, position);
    //return linearValueAt(from, to, position);
}

#define PI 3.141

int AnimatedMove::sinusValueAt(int from, int to, float position)
{
    float amplitude = (float)(to - from);
    float phase = -0.5*PI + position*PI;
    
    return from + (int)(amplitude *(1+sin(phase))/2);
}


QPoint AnimatedMove::currentPosition()
{
    int timeElapsed = m_cStartTime.elapsed(); // time elapsed in ms
    if(m_nTimeStep == 0){
        m_nLastPosition = 0;
        return QPoint(m_cFrom);
    }
    // position in percent
    float position = (float)timeElapsed / (float)m_nTimeStep;
    if(position > 1)
    {
        if(m_nLastPosition >= 1)
        {
            m_bAnimationFinished = TRUE;
        }
        position = 1;
        m_nLastPosition = position;
        return QPoint(m_cTo);
    }
    
    QPoint result(0, 0);
    result.setX(defaultValueAt(m_cFrom.x(), m_cTo.x(), position));
    result.setY(defaultValueAt(m_cFrom.y(), m_cTo.y(), position));
    m_nLastPosition = position;
    return result;
}

void AnimatedMove::start()
{
    m_cStartTime = QTime::currentTime();
    m_cStartTime.start();
    m_bAnimationFinished = FALSE;
}


void AnimatedMove::start(QPoint from, QPoint to)
{
    m_cFrom = from;
    m_cTo = to;
    start();
}


void AnimatedMove::setTimestep(int ms)
{
    m_nTimeStep = ms;
}

