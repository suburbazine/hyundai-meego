#ifndef QWERTYKEYPADWIDGET_H
#define QWERTYKEYPADWIDGET_H

#include <QtDeclarative/QDeclarativeItem>
#include <QGraphicsProxyWidget>

class QwertyKeypadWidget : public QGraphicsProxyWidget
{
    Q_OBJECT
    //Q_DISABLE_COPY(QwertyKeypadWidget)

public:
    QwertyKeypadWidget(QGraphicsProxyWidget *parent = 0);
    ~QwertyKeypadWidget();


    QRectF boundingRect() const
    {
        return QRectF(0, 0, 576, 524);
    }
};

QML_DECLARE_TYPE(QwertyKeypadWidget)

#endif // QWERTYKEYPADWIDGET_H

