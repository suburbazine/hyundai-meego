#include <QDebug>
#include <QGLFormat>

#include "AppGLWidget.h"

AppGLWidget* AppGLWidget::m_self = NULL;
//{added by junam 2013.04.03 for ISV77703
AppGLWidget* AppGLWidget::m_front = NULL;
AppGLWidget* AppGLWidget::m_rear = NULL;
//}added by junam

AppGLWidget::AppGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    qDebug() << isValid();

    //m_self = this;//removed by junam 2013.04.03 for ISV77703
}

AppGLWidget* AppGLWidget::instance()
{
    if (!m_self)
    {
        //{changed by junam 2013.04.03 for ISV77703
        //m_self = new AppGLWidget();
        m_self = instanceFront();
        //}changed by junam
    }
    return m_self;
}
//{added by junam 2013.04.03 for ISV77703
AppGLWidget* AppGLWidget::instanceFront()
{
    if (!m_front)
    {
        m_front = new AppGLWidget();
    }
    return m_front;
}

AppGLWidget* AppGLWidget::instanceRear()
{
    if (!m_rear)
    {
        m_rear = new AppGLWidget();
    }
    return m_rear;
}

void AppGLWidget::selectInstance(bool isFront)
{
    if(isFront)
    {
        if(m_self != m_front)
        {
            m_self->doneCurrent();
            m_self = m_front;
        }
    }
    else
    {
        if(m_self != m_rear)
        {
            m_self->doneCurrent();
            m_self = m_rear;
        }
    }
}
//}added by junam
void AppGLWidget::initializeGL()
{
    qDebug() << "initializeGL";
    emit initialize(this);
}

