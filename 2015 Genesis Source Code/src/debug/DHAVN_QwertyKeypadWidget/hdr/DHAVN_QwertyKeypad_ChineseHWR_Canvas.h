#ifndef QWERTY_KEYPAD_CHINESE_HWR_CANVAS_H
#define QWERTY_KEYPAD_CHINESE_HWR_CANVAS_H

#include <QtDeclarative/QDeclarativeItem>
#include <QObject>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QPen>


#ifndef DESKTOP_WINDOWS
    #include <persistman.h>
#endif

#include "DHAVN_QwertyKeypad_ChineseHWR.h"

class QwertyKeypadChineseHWR;

class QwertyKeypadChineseHWRCanvas : public QDeclarativeItem

{
    Q_OBJECT

public:
    explicit QwertyKeypadChineseHWRCanvas(QDeclarativeItem *parent = 0);
    ~QwertyKeypadChineseHWRCanvas();

    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

public:
    Q_INVOKABLE void initHWR();
    Q_INVOKABLE void releaseHWR();

    Q_INVOKABLE void initBtHWR();//added for ITS 229467 for BT Keypad HWR Engine Init Issue

    Q_INVOKABLE void sendLastPoint(int nMouseX, int nMouseY);
    Q_INVOKABLE void drawLineTo(int nMouseX, int nMouseY);
    Q_INVOKABLE void canvasRelease();
    Q_INVOKABLE void clearImage();
    Q_INVOKABLE void getDrawingPoints(int nMouseX, int nMouseY);
    Q_INVOKABLE QString getCandidate(const int nIndex);
    Q_INVOKABLE int getCandidateSize();
    Q_INVOKABLE void deleteCandidate();
    Q_INVOKABLE void startHWR();

    Q_INVOKABLE bool getDKBDPrediction(QString searchWord);
    Q_INVOKABLE int getTotalPageSize();
    Q_INVOKABLE int getCurrentPageNum();
    Q_INVOKABLE void incrementCurrentPage(bool isPrediction);
    Q_INVOKABLE void decrementCurrentPage(bool isPrediction);

    //int getInkListSize();

public:
    QwertyKeypadChineseHWR* mQwertyKeypadChineseHWR;
    QList<int>  mInkList;
    int*                 mInkListToHWR;
    int                   mInkListSize;

    QPoint          mLastPoint;
    QImage         mImage;
    QPen            mPen;

public slots:
    void updateCandidate(bool isPrediction);

signals:
    void candidateUpdated(bool isPrediction);
};


#endif // QWERTY_KEYPAD_CHINESE_HWR_CANVAS_H
