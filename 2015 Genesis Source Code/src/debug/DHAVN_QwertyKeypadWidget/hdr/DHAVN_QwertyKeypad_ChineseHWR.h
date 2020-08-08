#ifndef QWERTY_KEYPAD_CHINESE_HWR_H
#define QWERTY_KEYPAD_CHINESE_HWR_H

#include <QtDeclarative/QDeclarativeItem>
#include <QtSql>
#include "hwr/dhwr.h"

class QwertyKeypadChineseHWR: public QObject
{
    Q_OBJECT


public:
    explicit QwertyKeypadChineseHWR(QObject *parent = 0 );
     ~QwertyKeypadChineseHWR();

public:
    void makeInputEvent(DHWRInkObject ink, const int* inputs, const int nInkListSize);

    void initHWR();
    void initHWRforBT(); //added for ITS 229467 for BT Keypad HWR Engine Init Issue
    void releaseHWR();

    Q_INVOKABLE QString getCandidate(const int nIndex);
    Q_INVOKABLE int getCandidateSize();
    Q_INVOKABLE void deleteCandidate();

    Q_INVOKABLE int getTotalPageSize();
    Q_INVOKABLE int getCurrentPageNum();
    Q_INVOKABLE bool getDKBDPrediction(QString searchWord);
    Q_INVOKABLE void incrementCurrentPage(bool isPrediction);
    Q_INVOKABLE void decrementCurrentPage(bool isPrediction);

    void printWChar(const wchar_t* result, size_t size);
    void printResult(DHWRResultObject result);
    void recognize(const int* inputs, const int nInkListSize);

signals:
    void candidateUpdate(bool isPrediction);
    void predictionUpdate();

public:
    QStringList     mCandidateList;
    QStringList     mPredictionList;

private:
    void createKeyboardEngine();
    void clearInputString();
    void destroyKeyboardEngine();
    void setDKBDLibrary();

    void makeListByPage();
    void clearBuffer();

private:
    bool            m_bInit;

    DHWRInkObject 	m_inks;
    DHWRResultObject 	m_results;
    DHWRSettingObject 	m_hwrSetting;

    QList<QStringList> mPageList;
    bool            m_bCreatedDKBD;
    int             m_nCurrentPageNum;
    int             m_nPredictionTotalPage;
};

#endif // QWERTY_KEYPAD_CHINESE_HWR_H
