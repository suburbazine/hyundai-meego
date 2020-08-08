#include "DHAVN_QwertyKeypad_Widget.h"

#include <QtDeclarative/qdeclarative.h>

QwertyKeypadWidget::QwertyKeypadWidget(QGraphicsProxyWidget *parent):
        QGraphicsProxyWidget(parent)
{
    // By default, QDeclarativeItem does not draw anything. If you subclass
    // QDeclarativeItem to create a visual item, you will need to uncomment the
    // following line:

    // setFlag(ItemHasNoContents, false);

}

QwertyKeypadWidget::~QwertyKeypadWidget()
{
}

