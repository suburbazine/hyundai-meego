#include <QDebug>
#include <QGLFormat>

#include "AppGLWidget.h"

AppGLWidget* AppGLWidget::m_self = NULL;

AppGLWidget::AppGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    m_self = this;
}

AppGLWidget* AppGLWidget::instance()
{
    if (!m_self)
    {
        m_self = new AppGLWidget();
    }
    return m_self;
}

void AppGLWidget::release()
{
    if (m_self)
    {
        delete m_self;
        m_self = NULL;
    }
}

void AppGLWidget::initializeGL()
{
    emit initialize(this);
}
