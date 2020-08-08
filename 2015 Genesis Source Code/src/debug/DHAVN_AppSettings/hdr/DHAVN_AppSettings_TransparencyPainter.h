#ifndef DHAVN_APPSETTINGS_TRANSPARENCYPAINTER_H
#define DHAVN_APPSETTINGS_TRANSPARENCYPAINTER_H

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

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    Q_INVOKABLE void Update();

private:
    QColor m_color;
};

#endif // DHAVN_APPSETTINGS_TRANSPARENCYPAINTER_H
