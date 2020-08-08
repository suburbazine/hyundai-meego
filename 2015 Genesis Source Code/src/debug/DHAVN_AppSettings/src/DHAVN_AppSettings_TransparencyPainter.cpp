#include "DHAVN_AppSettings_TransparencyPainter.h"
#include <QPainter>
#include <QtOpenGL>

TransparencyPainter::TransparencyPainter(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    m_color.setRgba(qRgba(00,00,00,00));
    setColor(m_color);
}

void TransparencyPainter::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *w)
{
    // Paints the rectangle defined by the QML item with the exact given color, not applying
    // alpha but instead copying it as is.
    p->beginNativePainting();
    glEnable(GL_SCISSOR_TEST);

    QPointF tl = mapToScene(0, 0);
    //qDebug() << "TransparencyPainter::paint()";
    glScissor(tl.x(), w->height() - height() - tl.y(), width(), height());
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
    //qDebug() << "[SEREGA] Transparency Update";
    update();
}

