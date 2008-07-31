#include "ballposition.h"

BallPosition::BallPosition()
{
    m_nLogicalRow = 0;
    m_nLogicalCol = 0;
}

BallPosition::~BallPosition()
{
    
}


void BallPosition::setCurrentPosition(QPoint position)
{
    m_cCurrentPosition = position;
    m_cAnimation.start(m_cCurrentPosition, m_cCurrentPosition);
}

void BallPosition::moveTo(QPoint to)
{
    m_cAnimation.start(m_cCurrentPosition, to);
}

QPoint BallPosition::currentPosition() const
{
    return m_cCurrentPosition;
}


void BallPosition::updatePosition()
{
    m_cCurrentPosition = m_cAnimation.currentPosition();
}


