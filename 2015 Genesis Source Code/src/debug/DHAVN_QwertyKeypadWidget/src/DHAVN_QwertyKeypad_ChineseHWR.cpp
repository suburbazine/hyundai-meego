#include "DHAVN_QwertyKeypad_ChineseHWR.h"
#include "../hdr/dkbd/DKBD.h"
#include <QTime>
#include "DHAVN_QwertyKeypad_Log.h"

#define ITEM_COUNT_PER_PAGE 20
#define MAX_CANDIDATE_BUFFER_SIZE 2048

// { for Diotek Engine Log write to file
FILE* g_file = 0;

void EngineLogCallback(const unsigned short* msg)
{
    if (msg == 0) {
            qCritical("Engine Log Message is empty");
            return;
    }
    unsigned short *ch = const_cast<unsigned short*>(msg);
    int length = 0;
    while (*ch != 0) {
            ch++;
            length++;
    }

    if (g_file != 0) {
            fwrite((void*)msg, sizeof(unsigned short),length, g_file);
    }

}

void EngineLogFileOpen()
{
    g_file = fopen("/app/data/AppEngineeringMode/DiotekLog.txt", "w");
    if(g_file == 0){

        //qCritical() << "Diotek Engine Log File open failed !!!"; //added for Log System Lock Issue
        LOG2(QString("Diotek Engine Log File open failed !!!"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
}
void EngineLogFileClose()
{
    if(g_file != 0){
        fclose(g_file);
        g_file = 0;
    }
    else
    {
        //qCritical() << "Diotek Engine Log file already Closed ... ";//added for Log System Lock Issue
        LOG2(QString("Diotek Engine Log file already Closed ..."), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
}
// { for Diotek Engine Log write to file

QwertyKeypadChineseHWR::QwertyKeypadChineseHWR(QObject *parent): QObject(parent)
  ,m_bInit(false), m_bCreatedDKBD(false), m_nPredictionTotalPage(0), m_nCurrentPageNum(0)
{
    initHWR();

    //Create keyboard's engine
    createKeyboardEngine();

    //Keyboard's engine Setting
    setDKBDLibrary();

    // Keyboard's engine clear.
    clearInputString();
}

void QwertyKeypadChineseHWR::initHWR()
{
    //qCritical() << "QwertyKeypadChineseHWR::initHWR() ---- m_bInit:" << m_bInit;
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
    if (m_bInit == false)
    {
        unsigned long result = 0;
        m_inks = DHWRCreateInkObject();
        m_results = DHWRCreateResultObject();
        wchar_t symbol_set[] = L"?.,!@";

        result = DHWRCreate();

        // { for Diotek Engine Log write to file
        //DHWRLogLevel level = LEVEL_DEBUG;
        //DHWRSetParam(DHWR_LOG_LEVEL, &level);

        //EngineLogFileOpen();
        //DHWRSetParam(DHWR_LOG_CALLBACK, (void*)EngineLogCallback);
        // } for Diotek Engine Log write to file

        DHWRSetExternalResourcePath("/usr/lib/qt4/imports/QmlQwertyKeypadWidget/localization/Diotek/");

        m_hwrSetting = DHWRCreateSettingObject();
        DHWRSetWritingArea(m_hwrSetting, 0, 0, 677, 516, 100);
        DHWRSetRecognitionMode(m_hwrSetting, SINGLECHAR);
        DHWRSetCandidateSize(m_hwrSetting, 60/*DHWR_MAX_CANDIDATES*/); //modify for get CANDIDATE Count 59
        DHWRSetContinuity(m_hwrSetting, true);
        DHWRClearLanguage(m_hwrSetting);
        DHWRAddLanguage(m_hwrSetting, DLANG_SIMP_CHN, (DHWR_TYPE)(DTYPE_NONE));
        DHWRAddLanguage(m_hwrSetting, DLANG_ENGLISH, (DHWR_TYPE)(DTYPE_UPPERCASE | DTYPE_LOWERCASE));
        DHWRAddLanguage(m_hwrSetting, DLANG_NUMERIC, (DHWR_TYPE)(DTYPE_NONE));
        DHWRAddLanguage(m_hwrSetting, DLANG_PUNC, (DHWR_TYPE)(DTYPE_NONE)); // mixed with symbol
        //DHWRSetUserCharSet(m_hwrSetting, (unsigned short*)symbol_set);

        m_bInit = true;
    }
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}
//added for ITS 229467 for BT Keypad HWR Engine Init Issue
void QwertyKeypadChineseHWR::initHWRforBT()
{
    //qCritical() << "QwertyKeypadChineseHWR::initHWR() ---- m_bInit:" << m_bInit; //added for Log System Lock Issue
    //LOG2(QString("QwertyKeypadChineseHWR::initHWR() ---- m_bInit: %1").arg(m_bInit), Logger::PRIORITY_HI); //added for Log System Lock Issue
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
    if (m_bInit == false)
    {
        unsigned long result = 0;
        m_inks = DHWRCreateInkObject();
        m_results = DHWRCreateResultObject();
        wchar_t symbol_set[] = L"?.,!@";

        result = DHWRCreate();

        // { for Diotek Engine Log write to file
        //DHWRLogLevel level = LEVEL_DEBUG;
        //DHWRSetParam(DHWR_LOG_LEVEL, &level);

        //EngineLogFileOpen();
        //DHWRSetParam(DHWR_LOG_CALLBACK, (void*)EngineLogCallback);
        // } for Diotek Engine Log write to file

        DHWRSetExternalResourcePath("/usr/lib/qt4/imports/QmlQwertyKeypadWidget/localization/Diotek/");

        m_hwrSetting = DHWRCreateSettingObject();
        DHWRSetWritingArea(m_hwrSetting, 0, 0, 677, 516, 100);
        DHWRSetRecognitionMode(m_hwrSetting, SINGLECHAR);
        DHWRSetCandidateSize(m_hwrSetting, 60/*DHWR_MAX_CANDIDATES*/); //modify for get CANDIDATE Count 59
        DHWRSetContinuity(m_hwrSetting, true);
        DHWRClearLanguage(m_hwrSetting);
        DHWRAddLanguage(m_hwrSetting, DLANG_SIMP_CHN, (DHWR_TYPE)(DTYPE_NONE));
        DHWRAddLanguage(m_hwrSetting, DLANG_ENGLISH, (DHWR_TYPE)(DTYPE_UPPERCASE | DTYPE_LOWERCASE));
        DHWRAddLanguage(m_hwrSetting, DLANG_NUMERIC, (DHWR_TYPE)(DTYPE_NONE));
        DHWRAddLanguage(m_hwrSetting, DLANG_PUNC, (DHWR_TYPE)(DTYPE_NONE)); // mixed with symbol
        //DHWRSetUserCharSet(m_hwrSetting, (unsigned short*)symbol_set);

        m_bInit = true;
    }
    else if(m_bInit == true)
    {
        releaseHWR();

        unsigned long result = 0;
        m_inks = DHWRCreateInkObject();
        m_results = DHWRCreateResultObject();
        wchar_t symbol_set[] = L"?.,!@";

        result = DHWRCreate();

        // { for Diotek Engine Log write to file
        //DHWRLogLevel level = LEVEL_DEBUG;
        //DHWRSetParam(DHWR_LOG_LEVEL, &level);

        //EngineLogFileOpen();
        //DHWRSetParam(DHWR_LOG_CALLBACK, (void*)EngineLogCallback);
        // } for Diotek Engine Log write to file

        DHWRSetExternalResourcePath("/usr/lib/qt4/imports/QmlQwertyKeypadWidget/localization/Diotek/");

        m_hwrSetting = DHWRCreateSettingObject();
        DHWRSetWritingArea(m_hwrSetting, 0, 0, 677, 516, 100);
        DHWRSetRecognitionMode(m_hwrSetting, SINGLECHAR);
        DHWRSetCandidateSize(m_hwrSetting, 60/*DHWR_MAX_CANDIDATES*/); //modify for get CANDIDATE Count 59
        DHWRSetContinuity(m_hwrSetting, true);
        DHWRClearLanguage(m_hwrSetting);
        DHWRAddLanguage(m_hwrSetting, DLANG_SIMP_CHN, (DHWR_TYPE)(DTYPE_NONE));
        DHWRAddLanguage(m_hwrSetting, DLANG_ENGLISH, (DHWR_TYPE)(DTYPE_UPPERCASE | DTYPE_LOWERCASE));
        DHWRAddLanguage(m_hwrSetting, DLANG_NUMERIC, (DHWR_TYPE)(DTYPE_NONE));
        DHWRAddLanguage(m_hwrSetting, DLANG_PUNC, (DHWR_TYPE)(DTYPE_NONE)); // mixed with symbol
        //DHWRSetUserCharSet(m_hwrSetting, (unsigned short*)symbol_set);

        m_bInit = true;


    }
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

void QwertyKeypadChineseHWR::releaseHWR()
{
    //qCritical() << "QwertyKeypadChineseHWR::releaseHWR() ---- ";
    if (m_bInit)
    {
        DHWRDestroyInkObject(m_inks);
        DHWRDestroyResultObject(m_results);
        DHWRDestroySettingObject(m_hwrSetting);
        //qCritical()<<"DHWRClose()'s result:"<<DHWRClose(); //added for Log System Lock Issue
        LOG2(QString("DHWRClose()'s result: %1").arg(DHWRClose()), Logger::PRIORITY_HI); //added for Log System Lock Issue
        m_bInit = false;

        //EngineLogFileClose(); // { for Diotek Engine Log write to file
    }
}

void QwertyKeypadChineseHWR::createKeyboardEngine()
{
    //qCritical() << "QwertyKeypadChineseHWR::createKeyboardEngine() ---- ";
    if (DKBD_Create()) {
        m_bCreatedDKBD = true;
        //qCritical()<<"DKBD_Create() : success"; //added for Log System Lock Issue
        LOG2(QString("DKBD_Create() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        m_bCreatedDKBD = false;
        //qCritical()<<"DKBD_Create() : failed!!";//added for Log System Lock Issue
        LOG2(QString("DKBD_Create() : failed!!"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
}

void QwertyKeypadChineseHWR::destroyKeyboardEngine()
{
    //qCritical() << "QwertyKeypadChineseHWR::destroyKeyboardEngine() ---- ";
    if (DKBD_Destroy()) {
        m_bCreatedDKBD = false;
        //qCritical()<<"DKBD_Destroy() : success"; //added for Log System Lock Issue
        LOG2(QString("DKBD_Destroy() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        m_bCreatedDKBD = true;
        //qCritical()<<"DKBD_Destroy() : failed"; //added for Log System Lock Issue
        LOG2(QString("DKBD_Destroy() : failed"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
}

//입력된 문자열을 초기화 시킨다.
void QwertyKeypadChineseHWR::clearInputString()
{
    //qCritical() << "QwertyKeypadChineseHWR::clearInputString() ---- ";
    int status = DKBD_CNInputStrClean();
    if (status == HWERR_SUCCESS) {
        //qCritical()<<"DKBD_CNInputStrClean() : success"; //added for Log System Lock Issue
        LOG2(QString("DKBD_CNInputStrClean() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        //qCritical()<<"DKBD_CNInputStrClean() : failed!! status:"<< status; //added for Log System Lock Issue
        LOG2(QString("DKBD_CNInputStrClean() : failed!! status: %1").arg(status), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
}

// Setting DKBD Library.
void QwertyKeypadChineseHWR::setDKBDLibrary()
{
    //qCritical() << "QwertyKeypadChineseHWR::setDKBDLibrary() ---- ";
    int status = DKBD_CNSetDictPath("/usr/lib/qt4/imports/QmlQwertyKeypadWidget/localization/Diotek/pinyin/");

    if (status == HWERR_SUCCESS) {
        //qCritical()<<"DKBD_CNSetDictPath() : success"; //added for Log System Lock Issue
        LOG2(QString("DKBD_CNSetDictPath() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        //qCritical()<<"DKBD_CNSetDictPath() : failed"; //added for Log System Lock Issue
        LOG2(QString("DKBD_CNSetDictPath() : failed"), Logger::PRIORITY_HI); //added for Log System Lock Issue
        return;
    }

    //입력 언어를 설정한다.
    DKBD_SetLanguage(DKBD_LANG_SIMP_CHINESE);

    //Input method와 Input mode를 설정한다.
    int inputMethod = DKBD_INPUTMETHOD_QWERTY;
    int inputMode = DKBD_INPUTMODE_PHONETIC;
    DKBD_SetKeyboard(inputMethod, inputMode);

    //Pinyin fuzzy tone을 설정한다.
    int nFuzzy = HW_FUZZY_Z_ZH;
    status = DKBD_CNSetChineseFuzzy(nFuzzy);
    if (status == HWERR_SUCCESS) {
        //qCritical()<<"DKBD_CNSetChineseFuzzy() : success"; //added for Log System Lock Issue
        LOG2(QString("DKBD_CNSetChineseFuzzy() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        //qCritical()<<"DKBD_CNSetChineseFuzzy() : failed!! status:"<< status; //added for Log System Lock Issue
        LOG2(QString("DKBD_CNSetChineseFuzzy() : failed!! status: %1").arg(status), Logger::PRIORITY_HI); //added for Log System Lock Issue
        return;
    }
}

void QwertyKeypadChineseHWR::makeInputEvent(DHWRInkObject ink, const int* inputs, const int nInkListSize)
{
    //qCritical() << "QwertyKeypadChineseHWR::makeInputEvent() ---- ";
    DHWRInkClear(ink);

    unsigned long result = 0;

    for(int i=0; i < nInkListSize; i += 2)
    {
        if (inputs[i + 0] == -1 && inputs[i + 1] == 0)
        {
            DHWREndStroke(ink);
        }
        else
        {
            result = DHWRAddPoint(ink, inputs[i + 0], inputs[i + 1]);
        }

        if (result != ERR_SUCCESS)
            LOG2(QString("HWR  Add  FAIL!! result: %1").arg(result), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"HWR  Add  FAIL!! result:"<<result;
    }
    //result = DHWREndStroke(ink);
    if (result != ERR_SUCCESS)
        LOG2(QString("HWR  Add  FAIL"), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical("HWR  Add  FAIL"); //added for Log System Lock Issue
}

QString QwertyKeypadChineseHWR::getCandidate(const int nIndex)
{
    //qCritical() << "QwertyKeypadChineseHWR::getCandidate() ---- ";
    QString strCandidate;

    if (mPageList.isEmpty())
    {
        //qCritical("[CPP][ChineseHWR]getCandidate isEmpty !!!!!");
        LOG2(QString("getCandidate isEmpty !!!!!"), Logger::PRIORITY_HI); //added for Log System Lock Issue
        strCandidate = "";
    }
    else
    {
        if(nIndex < mPageList.at(m_nCurrentPageNum).length())
            strCandidate = mPageList.at(m_nCurrentPageNum).at(nIndex);
    }

    return strCandidate;
}

int QwertyKeypadChineseHWR::getCandidateSize()
{
    //qCritical() << "QwertyKeypadChineseHWR::getCandidateSize() ---- ";
    if (mPageList.isEmpty())
        return 0;
    else
        return mPageList.at(m_nCurrentPageNum).length();
}

int QwertyKeypadChineseHWR::getTotalPageSize()
{
    //qCritical() << "QwertyKeypadChineseHWR::getTotalPageSize() ---- ";
    if (mPageList.isEmpty())
        return 0;
    else
        return mPageList.length();
}

int QwertyKeypadChineseHWR::getCurrentPageNum()
{
    //qCritical() << "QwertyKeypadChineseHWR::getCurrentPageNum() ---- ";
    if (mPageList.isEmpty())
        return 0;
    else
        return m_nCurrentPageNum;
}

void QwertyKeypadChineseHWR::deleteCandidate()
{
    //qCritical() << "QwertyKeypadChineseHWR::deleteCandidate() ---- ";
    clearBuffer();
}

void QwertyKeypadChineseHWR::printWChar(const wchar_t* result, size_t size)
{
    //qCritical() << "QwertyKeypadChineseHWR::printWChar() ---- ";
    mCandidateList << QString::fromWCharArray(result, size);
}

void QwertyKeypadChineseHWR::printResult(DHWRResultObject result)
{
    //qCritical() << "QwertyKeypadChineseHWR::printResult() ---- ";
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
    deleteCandidate();

    bool is_exit = false;
    size_t length;
    size_t size_line = DHWRGetLineSize(result);
    size_t size_block;
    DHWRResultLine line;
    DHWRResultBlock block;
    const wchar_t* current_cand;

    if (size_line == 0)
    {
        //qCritical()<<"[CPP][ChineseHWR]printResult() - result empty";
        LOG2(QString("printResult() - result empty"), Logger::PRIORITY_HI); //added for Log System Lock Issue
        return;
    }
    for (size_t i = 0; i < 59; i++) //modify for get CANDIDATE Count 59
    {
        for (size_t j = 0; j < size_line; j++)
        {
            line = DHWRGetLine(result, j);
            size_block = DHWRGetBlockSize(line);
            for (size_t k = 0; k < size_block; k++)
            {
                block = DHWRGetBlock(line, k);
                if (DHWRGetCandidateSize(block) <= i)
                {
                    is_exit = true;
                    break;
                }
                current_cand = (const wchar_t*)(DHWRGetCandidate(block, i, &length));

                printWChar(current_cand, length);
            }
            if (is_exit == true)
                break;
        }
        if (is_exit == true)
            break;
    }

    // 화면에 표시
    if(mCandidateList.length())
    {
        makeListByPage();
    }

    emit candidateUpdate(false);
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

void QwertyKeypadChineseHWR::recognize(const int* inputs, const int nInkListSize)
{
    //qCritical() << "QwertyKeypadChineseHWR::recognize() ---- ";
    makeInputEvent(m_inks, inputs, nInkListSize);

    DHWRSetAttribute(m_hwrSetting);

    int result = DHWRecognize(m_inks, m_results);

    if (result == ERR_SUCCESS)
        printResult(m_results);
    else
        LOG2(QString("HWR result Empty !!!: result: %1").arg(result), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical()<<"[CPP][ChineseHWR]HWR result Empty !!!: result:"<<result;

    //    DHWRDestroyInkObject(inks);
    //    DHWRDestroyResultObject(results);
    //    DHWRDestroySettingObject(chinese_setting);

    //    DHWRClose();
}

bool QwertyKeypadChineseHWR::getDKBDPrediction(QString searchWord)
{
    //qCritical()<<"HWR getDKBDPrediction: searchWord:"<<searchWord;
    if(searchWord == "")
        return false;

    int nCandidateNum = 0;
    unsigned short outString[HW_MAX_INPUT_SHOW_LEN] = {0,};
    unsigned short candWordBuf[MAX_CANDIDATE_BUFFER_SIZE] = {0,};

    for(int i = 0; i < searchWord.length(); i++) {
        outString[i] = searchWord[i].unicode();
    }

    nCandidateNum = DKBD_CNGetPredictResult(outString, NULL, candWordBuf, MAX_CANDIDATE_BUFFER_SIZE);
    LOG2(QString("PredictResult() : Number of candidate: %1").arg(nCandidateNum), Logger::PRIORITY_HI); //added for Log System Lock Issue
    //qCritical()<<"PredictResult() : Number of candidate:"<< nCandidateNum;

    // 다음 후보군이 존재할경우
    if( nCandidateNum > 0 )
    {
        // 초기화
        clearBuffer();

        // 리스트에 추가
        QString tempCandidate = "";
        for (int i = 0, j = 0; j < nCandidateNum; i++) {
            if (candWordBuf[i] != 0)
            {
                tempCandidate.append(QString::fromUtf16(&candWordBuf[i], 1));
            }
            else
            {
                if(mCandidateList.length() > 58) // 후보군의 경우 3 Page를 넘기지 않음. 마지막 페이지에 마지막 항목은 비움. 1 Page(20 char) (20*3-1)
                    break;

                if(tempCandidate.length() == 1)
                {
                    mCandidateList.append(tempCandidate);
                }
                tempCandidate = "";
                j++;
            }
        }

        // 화면에 표시
        if(mCandidateList.length())
        {
            makeListByPage();
        }

        emit candidateUpdate(true);

        return true;
    }
    else // 다음 후보군이 존재하지 않을 경우
    {
        clearBuffer();

        emit candidateUpdate(true);

        return false;
    }
}

QwertyKeypadChineseHWR::~QwertyKeypadChineseHWR()
{
    //qCritical() << "QwertyKeypadChineseHWR::~QwertyKeypadChineseHWR() ---- ";
    releaseHWR();
}

// 다음 페이지 후보군 리스트를 표시한다. 마지막 페이지일경우 첫 페이지 후보군 리스트를 표시한다.
void QwertyKeypadChineseHWR::incrementCurrentPage(bool isPrediction)
{
    //qCritical() << "QwertyKeypadChineseHWR::incrementCurrentPage() ---- ";
    if(mCandidateList.length() && mPageList.length())
    {
        if(m_nCurrentPageNum < m_nPredictionTotalPage)
            m_nCurrentPageNum++;
        else
            m_nCurrentPageNum = 0;
    }

    if(isPrediction)
        emit candidateUpdate(true);
    else
        emit candidateUpdate(false);
}

// 이전 페이지 후보군 리스트를 표시한다. 첫 페이지일경우 마지막 페이지 후보군 리스트를 표시한다.
void QwertyKeypadChineseHWR::decrementCurrentPage(bool isPrediction)
{
    //qCritical() << "QwertyKeypadChineseHWR::decrementCurrentPage() ---- ";
    if(mCandidateList.length() && mPageList.length())
    {
        if(m_nCurrentPageNum > 0)
            m_nCurrentPageNum--;
        else
            m_nCurrentPageNum = mPageList.length()-1;
    }

    if(isPrediction)
        emit candidateUpdate(true);
    else
        emit candidateUpdate(false);
}

// 전체 후보군 리스트를 페이지별로 분리하여 저장한다.
void QwertyKeypadChineseHWR::makeListByPage()
{
    //qCritical() << "QwertyKeypadChineseHWR::makeListByPage() ---- ";
    int sumTotal = 0;
    QStringList tempStringList;

    // 전체 페이지 초기화
    m_nPredictionTotalPage = 0;

    for(int i=0; i < mCandidateList.length(); i++)
    {
        sumTotal ++;

        // 1. 20칸을 넘었을 경우.
        if(sumTotal > 20)
        {
            //합계 초기화
            sumTotal = 0;

            // 전체 페이지 증가
            m_nPredictionTotalPage++;

            // 페이지 리스트에 지금까지 추가한 문자열 리스트를 추가 등록한다
            mPageList.append(tempStringList);

            // 문자열 리스트를 초기화
            if(tempStringList.length())
                tempStringList.clear();

            // 현재 문자열을 저장
            tempStringList.append(mCandidateList.at(i));

            // 한 페이지 안에 들어갈 수 있는 사이즈는 최대 8칸(버튼기준)
            sumTotal ++;

            //마지막 항목일 경우 페이지 리스트에 저장
            if(i == mCandidateList.length()-1)
                mPageList.append(tempStringList);
        }
        else // 2. 합계가 20칸일경우 문자열 리스트에 글자를 추가 등록한다.
        {
            tempStringList.append(mCandidateList.at(i));

            //마지막 항목일 경우 페이지 리스트에 저장
            if(i == mCandidateList.length()-1)
                mPageList.append(tempStringList);
        }
    }
}

void QwertyKeypadChineseHWR::clearBuffer()
{
    //qCritical() << "QwertyKeypadChineseHWR::clearBuffer() ---- ";
    m_nPredictionTotalPage = 0;
    m_nCurrentPageNum = 0;

    if(mCandidateList.length() && mPageList.length())
    {
        mPageList.clear();
        mCandidateList.clear();
    }

    clearInputString();
}
