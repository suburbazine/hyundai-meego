#ifndef TRANSPARENCYPAINTER_H
#define TRANSPARENCYPAINTER_H

#include <QDeclarativeItem>
#include <QColor>
class TransparencyPainter : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    explicit TransparencyPainter(QDeclarativeItem *parent = 0);
    QColor color();
    void setColor(QColor color);

    Q_INVOKABLE void Update();

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

private:
    QColor m_color;
};

#endif // TRANSPARENCYPAINTER_H
