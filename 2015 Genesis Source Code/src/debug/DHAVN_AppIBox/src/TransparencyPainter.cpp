#include "TransparencyPainter.h"
#include <QPainter>
#include <QtOpenGL>
#include <QDebug>

TransparencyPainter::TransparencyPainter(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    m_color.setRgba(qRgba(0, 0, 0, 0));
}

void TransparencyPainter::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *w)
{
    p->beginNativePainting();

    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, 1280, 720);
    glClearColor(m_color.redF(), m_color.greenF(), m_color.blueF(), m_color.alphaF());
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    p->endNativePainting();
}

void TransparencyPainter::setColor(QColor color)
{
    m_color = color;
}

QColor TransparencyPainter::color()
{
    return m_color;
}

void TransparencyPainter::Update()
{
   this->update();
}
