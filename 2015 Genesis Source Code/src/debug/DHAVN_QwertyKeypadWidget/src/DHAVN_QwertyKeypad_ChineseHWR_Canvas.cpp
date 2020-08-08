#include "DHAVN_QwertyKeypad_ChineseHWR_Canvas.h"
#include "DHAVN_QwertyKeypad_ChineseHWR.h"
#include <QApplication>
#include <QDebug>
#include <QtGui>
#include <QTime>
#include "DHAVN_QwertyKeypad_Log.h"
QColor PEN_COLOR = Qt::black;
#define PEN_WIDTH 8
#define CANVAS_WIDTH 677
#define CANVAS_HEIGHT 516

QwertyKeypadChineseHWRCanvas::QwertyKeypadChineseHWRCanvas(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      mInkListToHWR(0),
      mInkListSize(0),
      mPen(PEN_COLOR, PEN_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{
    //qDebug() << "[LBG] "  << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
    //setCacheMode(QGraphicsItem::ItemCoordinateCache, QSize(100, 100));
    mQwertyKeypadChineseHWR = NULL;

    setFlag(QGraphicsItem::ItemHasNoContents, false);
    mImage = QImage( CANVAS_WIDTH, CANVAS_HEIGHT, QImage::Format_ARGB32);

    mInkList = QList<int>();
    mQwertyKeypadChineseHWR = new QwertyKeypadChineseHWR;

    update(0,0,CANVAS_WIDTH,CANVAS_HEIGHT);

    connect ( mQwertyKeypadChineseHWR, SIGNAL(candidateUpdate(bool)),
              this, SLOT(updateCandidate(bool)));

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

void QwertyKeypadChineseHWRCanvas::initHWR()
{
    //qCritical() << "initHWR() --------";
    if (mQwertyKeypadChineseHWR == NULL)
        return;

    mQwertyKeypadChineseHWR->initHWR();
}

//added for ITS 229467 for BT Keypad HWR Engine Init Issue
void QwertyKeypadChineseHWRCanvas::initBtHWR()
{
    //qCritical() << "initBtHWR() --------";
    LOG2(QString("initBtHWR()"), Logger::PRIORITY_HI); //added for Log System Lock Issue

    mQwertyKeypadChineseHWR->initHWRforBT();
}

void QwertyKeypadChineseHWRCanvas::releaseHWR()
{
    //qCritical() << "releaseHWR() --------";
    if (mQwertyKeypadChineseHWR == NULL)
        return;

    mQwertyKeypadChineseHWR->releaseHWR();
}

QString QwertyKeypadChineseHWRCanvas::getCandidate(const int nIndex)
{
    //qCritical() << "getCandidate() --------";
    if (mQwertyKeypadChineseHWR == NULL)
        return QString();

    return mQwertyKeypadChineseHWR->getCandidate(nIndex);;
}

int QwertyKeypadChineseHWRCanvas::getCandidateSize()
{
    //qCritical() << "getCandidateSize() --------";
    return mQwertyKeypadChineseHWR->getCandidateSize();
}

void QwertyKeypadChineseHWRCanvas::deleteCandidate()
{
    //qCritical() << "deleteCandidate() --------";
    if (mQwertyKeypadChineseHWR == NULL)
        return;

    mQwertyKeypadChineseHWR->deleteCandidate();
}

int QwertyKeypadChineseHWRCanvas::getCurrentPageNum()
{
    //qCritical() << "getCurrentPageNum() --------";
    if (mQwertyKeypadChineseHWR == NULL)
        return 0;
    else
        return mQwertyKeypadChineseHWR->getCurrentPageNum();
}

int QwertyKeypadChineseHWRCanvas::getTotalPageSize()
{
    //qCritical() << "getTotalPageSize() --------";
    if (mQwertyKeypadChineseHWR == NULL)
        return 0;
    else
        return mQwertyKeypadChineseHWR->getTotalPageSize();
}

void QwertyKeypadChineseHWRCanvas::clearImage()
{
    //qCritical() << "clearImage() --------";
    mInkList.clear();
//    mImage = QImage( CANVAS_WIDTH, CANVAS_HEIGHT, QImage::Format_ARGB32);
    mImage.fill(Qt::transparent);
    update();
}

void QwertyKeypadChineseHWRCanvas::getDrawingPoints(int nMouseX, int nMouseY)
{
    //qCritical() << "getDrawingPoints() --------";
    mInkList.append(nMouseX);
    mInkList.append(nMouseY);

    //mInkListSize = mInkList.size();
}

void QwertyKeypadChineseHWRCanvas::startHWR()
{
    //qCritical() << "startHWR() --------";
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
    if (mQwertyKeypadChineseHWR ==  NULL)
        return;

    mInkList.append(-1);
    mInkList.append(0);

    int mInkListSize = mInkList.size();

    if (mInkListToHWR)
    {
        delete [] mInkListToHWR;
        mInkListToHWR = NULL;
    }

    mInkListToHWR = new int[mInkListSize];

    for(int i=0; i<mInkListSize; i++)
    {
        mInkListToHWR[i] = 0;
    }

    for(int i=0; i < mInkListSize; i++)
    {
        mInkListToHWR[i] = mInkList.at(i);

        //qCritical() << "[QT]Keypad::sendPoint: " << mInkListToHWR[i];
    }
    mQwertyKeypadChineseHWR->recognize(mInkListToHWR, mInkListSize);
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

void QwertyKeypadChineseHWRCanvas::sendLastPoint(int nMouseX, int nMouseY)
{
    //qCritical() <<"[QT]KeyPadWidget:: sendLastPoint :" << nMouseX << ", " << nMouseY;
    mLastPoint = QPoint(nMouseX, nMouseY);

    QPainter painter(&mImage);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(mPen);
    painter.drawPoint(mLastPoint);
}

void QwertyKeypadChineseHWRCanvas::drawLineTo(int nMouseX, int nMouseY)
{
    //qCritical() <<"[QT]KeyPadWidget:: drawLineTo :" << nMouseX << ", " << nMouseY;
    QPainter painter(&mImage);
    QPoint endPoint = QPoint(nMouseX, nMouseY);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(mPen);
    painter.drawLine(mLastPoint, endPoint);

    int rad = (PEN_WIDTH / 2) + 2;

    update(QRect(mLastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

    mLastPoint = endPoint;
}

void QwertyKeypadChineseHWRCanvas::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *w)
{
    //qCritical() << "paint() --------";
    QRect dirtyRect = w->rect();
    p->drawImage(dirtyRect, mImage, dirtyRect);
}

QwertyKeypadChineseHWRCanvas::~QwertyKeypadChineseHWRCanvas()
{
    //qCritical() << "~QwertyKeypadChineseHWRCanvas() --------";
    if(!mInkList.isEmpty())
        mInkList.clear();

    if(mInkListToHWR != NULL)
    {
        delete [] mInkListToHWR;
        mInkListToHWR = NULL;
    }

    if(mQwertyKeypadChineseHWR != NULL)
        delete mQwertyKeypadChineseHWR;
}

void QwertyKeypadChineseHWRCanvas::updateCandidate(bool isPrediction)
{
    //qCritical() << "updateCandidate() --------";
    emit candidateUpdated(isPrediction);
}

bool QwertyKeypadChineseHWRCanvas::getDKBDPrediction(QString searchWord)
{
    //qCritical() << "getDKBDPrediction() --------";
    if (mQwertyKeypadChineseHWR == NULL)
        return false;

    return mQwertyKeypadChineseHWR->getDKBDPrediction(searchWord);
}

// 다음 페이지 후보군 리스트를 표시한다. 마지막 페이지일경우 첫 페이지 후보군 리스트를 표시한다.
void QwertyKeypadChineseHWRCanvas::incrementCurrentPage(bool isPrediction)
{
    //qCritical() << "incrementCurrentPage() --------";
    if (mQwertyKeypadChineseHWR == NULL)
        return;

    mQwertyKeypadChineseHWR->incrementCurrentPage(isPrediction);
}

// 이전 페이지 후보군 리스트를 표시한다. 첫 페이지일경우 마지막 페이지 후보군 리스트를 표시한다.
void QwertyKeypadChineseHWRCanvas::decrementCurrentPage(bool isPrediction)
{
    //qCritical() << "decrementCurrentPage() --------";
    if (mQwertyKeypadChineseHWR == NULL)
        return;

    mQwertyKeypadChineseHWR->decrementCurrentPage(isPrediction);
}
