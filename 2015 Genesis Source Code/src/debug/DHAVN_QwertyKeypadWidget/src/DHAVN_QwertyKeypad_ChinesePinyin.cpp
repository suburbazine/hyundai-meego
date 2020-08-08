#include "DHAVN_QwertyKeypad_ChinesePinyin.h"
#include <QTime>
#include "DHAVN_QwertyKeypad_Log.h"
#include <stdio.h>
#include <string.h>
#include "../hdr/dkbd/DKBD.h"

#define MAX_CANDIDATE_BUFFER_SIZE 2048

#define DIO_CHINESEIME 1

#define KEYPAD_PINYIN 0
#define KEYPAD_VOCAL_SOUND 1

QwertyKeypadChinesePinyin::QwertyKeypadChinesePinyin(QObject *parent) : QAbstractListModel(parent)
  ,m_bUsedPhoneBookDB(false), m_PhoneBookDBPath(""), m_bCreatedDKBD(false), m_nPinyinCount(0)
  ,mKeypadType(-1),m_nCurrentPageNum(0),m_nPinyinTotalPage(0)
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());

    QHash<int, QByteArray> roles = roleNames();
    roles.insert( nameRole,  QByteArray( "name" ) );
    roles.insert( sizeRole,  QByteArray( "size" ) );
    setRoleNames(roles);

    if(!mDB.isValid())
    {
        mDB = QSqlDatabase::addDatabase("QSQLITE");
    }

    //Create keyboard's engine
    createKeyboardEngine();

    //Keyboard's engine Setting
    setDKBDLibrary();

    // Keyboard's engine clear.
    clearInputString();

    phoneticSizeRange = 0;  //added for Static test fail 03/07

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

QwertyKeypadChinesePinyin::~QwertyKeypadChinesePinyin()
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
    if(mDB.isOpen())
    {
        mDB.close();
        mDB.removeDatabase(QSqlDatabase::defaultConnection);
    }

    //    if(m_bCreatedDKBD)
    //        destroyKeyboardEngine();
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

void QwertyKeypadChinesePinyin::createKeyboardEngine()
{
    if (DKBD_Create()) {
        m_bCreatedDKBD = true;
        //qCritical()<<"DKBD_Create() : success";
        LOG2(QString("DKBD_Create() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        m_bCreatedDKBD = false;
        //qCritical()<<"DKBD_Create() : failed!!";
        LOG2(QString("DKBD_Create() : failed!!"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
}

void QwertyKeypadChinesePinyin::destroyKeyboardEngine()
{
    if (DKBD_Destroy()) {
        m_bCreatedDKBD = false;
        //qCritical()<<"DKBD_Destroy() : success";
        LOG2(QString("DKBD_Destroy() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        m_bCreatedDKBD = true;
        //qCritical()<<"DKBD_Destroy() : failed";
        LOG2(QString("DKBD_Destroy() : failed"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
}

//입력된 문자열을 초기화 시킨다.
void QwertyKeypadChinesePinyin::clearInputString()
{
    int status = DKBD_CNInputStrClean();
    if (status == HWERR_SUCCESS) {
        //qCritical()<<"DKBD_CNInputStrClean() : success";
        LOG2(QString("DKBD_CNInputStrClean() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        //qCritical()<<"DKBD_CNInputStrClean() : failed!! status:"<< status;
        LOG2(QString("DKBD_CNInputStrClean() : failed!! status: %1").arg(status), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
}

// Setting DKBD Library.
void QwertyKeypadChinesePinyin::setDKBDLibrary()
{
    int status = DKBD_CNSetDictPath("/usr/lib/qt4/imports/QmlQwertyKeypadWidget/localization/Diotek/pinyin/");

    if (status == HWERR_SUCCESS) {
        //qCritical()<<"DKBD_CNSetDictPath() : success";
        LOG2(QString("DKBD_CNSetDictPath() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        //qCritical()<<"DKBD_CNSetDictPath() : failed";
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
        //qCritical()<<"DKBD_CNSetChineseFuzzy() : success";
        LOG2(QString("DKBD_CNSetChineseFuzzy() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    } else {
        //qCritical()<<"DKBD_CNSetChineseFuzzy() : failed!! status:"<< status;
        LOG2(QString("DKBD_CNSetChineseFuzzy() : failed!! status: %1").arg(status), Logger::PRIORITY_HI); //added for Log System Lock Issue
        return;
    }
}

void QwertyKeypadChinesePinyin::setKeypadType(int nType, bool bPhoneBook, QString phoneBookDBPath)
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());

    //qCritical()<<"[Keypad]setKeypadType():nType"<<nType<<", bPhoneBook:"<<bPhoneBook<<", phoneBookDBPath:"<<phoneBookDBPath;
    LOG2(QString("setKeypadType():nType[%1],bPhoneBook:[%2], phoneBookDBPath:[%3]").arg(nType)
         .arg(bPhoneBook).arg(phoneBookDBPath), Logger::PRIORITY_HI); //added for Log System Lock Issue

    if(mKeypadType == nType)
        return;

    mKeypadType = nType;

    if(mDB.isOpen())
    {
        //qCritical()<<"[Keypad]setKeypadType():mDB.isOpen()=>"<<mDB.isOpen()<<":=>Excute mDB.close()";
        LOG2(QString("setKeypadType():mDB.isOpen()=> %1 :=>Excute mDB.close()").arg(mDB.isOpen()), Logger::PRIORITY_HI); //added for Log System Lock Issue
        mDB.close();
    }

    if(!m_bCreatedDKBD)
    {
        //Create keyboard's engine
        createKeyboardEngine();

        //Keyboard's engine Setting
        setDKBDLibrary();

        // Keyboard's engine clear.
        clearInputString();
    }

    if(mKeypadType == KEYPAD_VOCAL_SOUND)
    {
        if( bPhoneBook && (phoneBookDBPath != "") )
            connectBTPhoneBookDB(bPhoneBook, phoneBookDBPath);
        else
            connectMandarinDB();
    }

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

void QwertyKeypadChinesePinyin::connectBTPhoneBookDB(bool bPhoneBook, QString phoneBookDBPath)
{
    if( !(bPhoneBook) || (phoneBookDBPath == ""))
        return;

    m_bUsedPhoneBookDB = bPhoneBook;

    QFile dbFile(phoneBookDBPath);
    if(dbFile.exists())
    {
        //qCritical()<<"[Keypad]PhoneBook's DB file is exists.";
        //LOG2(QString("PhoneBook's DB file is exists."), Logger::PRIORITY_HI); //added for Log System Lock Issue
        m_PhoneBookDBPath = phoneBookDBPath;
        mDB.setDatabaseName(phoneBookDBPath);
    }
    else
    {
        //qCritical()<<"[Keypad]PhoneBook's DB open failed!! DB file is not exists";
        LOG2(QString("PhoneBook's DB open failed!! DB file is not exists"), Logger::PRIORITY_HI); //added for Log System Lock Issue
        m_bUsedPhoneBookDB = false;
        emit phoneBookDBOpenFailed();
    }

    // DB Open && Retry Open when DB is not opened.
    bool isOpenSuccess = false;
    int nOpenAttemptCount = 0;

    while(true)
    {
        // DB Open
        isOpenSuccess = mDB.open();
        nOpenAttemptCount++;

        if(isOpenSuccess)
        {
            //qCritical()<<"[Keypad]PhoneBook's DB Open Success. DBName:"<< mDB.databaseName();
            //LOG2(QString("[Keypad]PhoneBook's DB Open Success. DBName: %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue
            break;
        }
        else
        {
            //qCritical()<<"[Keypad]PhoneBook's DB is not opened!! DBName:"<< mDB.databaseName();
            LOG2(QString("PhoneBook's DB is not opened!! DBName: %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue
            if(nOpenAttemptCount > 3)
            {
                m_bUsedPhoneBookDB = false;
                emit phoneBookDBOpenFailed();

                break;
            }
        }
    }
}

void QwertyKeypadChinesePinyin::connectMandarinDB()
{
    mDB.setDatabaseName("/usr/lib/qt4/imports/QmlQwertyKeypadWidget/localization/mandarin.db");

    bool isOpenSuccess = false;
    int nOpenAttemptCount = 0;

    while(true)
    {
        // DB Open
        isOpenSuccess = mDB.open();
        nOpenAttemptCount++;

        if(isOpenSuccess)
        {
            //qCritical()<<"[Keypad]DB Open Success. DBName:"<< mDB.databaseName();
            //LOG2(QString("[Keypad]DB Open Success. DBName: %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue
            break;
        }
        else
        {
            //qCritical()<<"[Keypad]DB is not opened!! DBName:"<< mDB.databaseName();
            LOG2(QString("DB is not opened!! DBName: %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue

            if(nOpenAttemptCount > 3)
            {
                if(m_bUsedPhoneBookDB)
                    m_bUsedPhoneBookDB = false;

                emit phoneBookDBOpenFailed();

                break;
            }
        }
    }
}

QStringList QwertyKeypadChinesePinyin::getConsonantsNextCharList()
{
    return mNextCharList;
}

void QwertyKeypadChinesePinyin::setKeyWord(QString keyWord)
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());

    //qCritical()<<"[Keypad]setKeyWord(): m_bCreatedDKBD:" << m_bCreatedDKBD;
    LOG2(QString("[Keypad]setKeyWord(): m_bCreatedDKBD: %1").arg(m_bCreatedDKBD), Logger::PRIORITY_HI); //added for Log System Lock Issue
    bool isSuccess = false;

    if(m_bUsedPhoneBookDB)
    {
        if(mDB.isOpen())
            LOG2(QString("setKeyWord(): DB is opened. DBName=> %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"[Keypad]setKeyWord():DB is opened. DBName=>" << mDB.databaseName();

        else
        {
            LOG2(QString("setKeyWord():DB is not opened.=> %1").arg(mDB.lastError().text()), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"[Keypad]setKeyWord():DB is not opened. mDB.lastError().text()=>" << mDB.lastError().text();

            mDB = QSqlDatabase::database();
            bool isOpen = mDB.open();

            if(!isOpen)
            {
                //qCritical()<<"[Keypad]setKeyWord():DB reopen failed!! mDB.lastError().text():" << mDB.lastError().text();
                LOG2(QString("setKeyWord():DB reopen failed: %1").arg(mDB.lastError().text()), Logger::PRIORITY_HI); //added for Log System Lock Issue
            }
        }
    }

    clearBuffer();

    if(!keyWord.isEmpty())
    {
        // 성음
        if(mKeypadType == KEYPAD_VOCAL_SOUND)
        {
            // 폰북검색
            if(m_bUsedPhoneBookDB)
            {
                // return when is used PhoneBook's DB
                setKeyWordPhoneBook(keyWord);
                return;
            }
            else // 만다린
            {
                // 데이터베이스 연결 (실패시 재시도)
                if(mDB.isOpen())
                    LOG2(QString("setKeyWord():DB is opened. DBName= %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue
                    //qCritical()<<"[Keypad]setKeyWord():DB is opened. DBName=>" << mDB.databaseName();
                else
                {
                    LOG2(QString("setKeyWord():DB is not opened.=> %1").arg(mDB.lastError().text()), Logger::PRIORITY_HI); //added for Log System Lock Issue
                    //qCritical()<<"[Keypad]setKeyWord():DB is not opened. mDB.lastError().text()=>" << mDB.lastError().text();

                    mDB = QSqlDatabase::database();
                    bool isOpen = mDB.open();

                    if(!isOpen)
                    {
                        LOG2(QString("setKeyWord():DB reopen failed: %1").arg(mDB.lastError().text()), Logger::PRIORITY_HI); //added for Log System Lock Issue
                        //qCritical()<<"[Keypad]setKeyWord():DB reopen failed!! mDB.lastError().text():" << mDB.lastError().text();
                        return;
                    }
                }

                // 다음 글자 활성화
                setKeyWordConsonants(keyWord);

                // 만다린 후보군
                QSqlQuery query;           // 0x2018
                QString cmd = QString("SELECT * FROM dictionary WHERE  typed='%1' ORDER BY \"freq\" DESC").arg(keyWord.toLower().replace(QChar(0x02BC),QString(" ")));
                if(query.exec(cmd))
                {
                    if(query.first())
                    {
                        do
                        {
                            addItem(query.value(0).toString(), query.value(0).toString().length());
                            mList.append(query.value(0).toString());
                            m_nPinyinCount++;
                        }while(query.next());
                    }

                    isSuccess = true;
                }
                else
                    isSuccess = false;

                // 만다린 후보군 페이지 리스트
                if(isSuccess)
                    makeListByPage();
            }
        }
        else // 병음 (디오텍 엔진)
        {
            isSuccess = getDKBDCandidate(keyWord);

            if(isSuccess)
                makeListByPage();
        }
    }
    else
    {
        LOG2(QString("keyWord is Empty."), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical()<<"[Keypad]setKeyWord():keyWord is Empty.";
    }


    if(mList.length() && mPageList.length() && isSuccess )
    {
        m_nCurrentPageNum = 0;
        beginInsertRows( QModelIndex(), 0, mPageList.at(m_nCurrentPageNum).length()-1 );
        endInsertRows();
    }

    emit pinyinCountChanged();

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

void QwertyKeypadChinesePinyin::setKeyWordPhoneBook(QString keyWord)
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
    if(mDB.isOpen())
        LOG2(QString("setKeyWordPhoneBook():DB is opened: %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical()<<"[Keypad]setKeyWordPhoneBook():DB is opened!! DBName:"<< mDB.databaseName();
    else
    {
        LOG2(QString("setKeyWordPhoneBook():DB is not opened: %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical()<<"[Keypad]setKeyWordPhoneBook():DB is not opened!! DBName:"<< mDB.databaseName();
        return;
    }

    if(!keyWord.isEmpty())
    {
        QStringList consonantList;
        QStringList nextCharList;
        QString currentNextChar;
        QSqlQuery consonantQuery;

        QString consonantCmd = "";
        consonantCmd = QString("SELECT sound FROM vocal_sound WHERE sound LIKE \"%" + keyWord.toUpper() + "%\" ");

        int nPos = keyWord.length();
        int i;

        if(consonantQuery.exec(consonantCmd))
        {
            if(consonantQuery.first())
            {
                do
                {
                    consonantList.append(consonantQuery.value(0).toString());
                }while(consonantQuery.next());
            }

            if (!consonantList.isEmpty())
            {
                int index = -1;

                for(i=0; i < consonantList.length(); i++)
                {
                    if ( keyWord != consonantList.at(i) )
                    {
                        index = consonantList.at(i).indexOf(keyWord.toUpper(), 0);

                        if ( currentNextChar != consonantList.at(i).at(index+keyWord.length()))
                        {
                            nextCharList.append(consonantList.at(i).at(index+keyWord.length()).toUpper());
                            currentNextChar = consonantList.at(i).at(index+keyWord.length()).toUpper();
                        }
                    }
                }
                mNextCharList = nextCharList;
            }
        }
    }
    else
    {
        LOG2(QString("setKeyWordPhoneBook():keyWord is Empty."), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical()<<"[Keypad]setKeyWordPhoneBook():keyWord is Empty.";
    }

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

void QwertyKeypadChinesePinyin::setKeyWordConsonants(QString keyWord)
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
    if(mDB.isOpen())
        LOG2(QString("setKeyWordConsonants():DB is opened: %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical()<<"[Keypad]setKeyWordConsonants():DB is opened!! DBName:"<< mDB.databaseName();
    else
    {
        LOG2(QString("setKeyWordConsonants():DB is not opened: %1").arg(mDB.databaseName()), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical()<<"[Keypad]setKeyWordConsonants():DB is not opened!! DBName:"<< mDB.databaseName();
        return;
    }

    if(!keyWord.isEmpty())
    {
        QStringList consonantList;
        QStringList nextCharList;
        QString currentNextChar;
        QSqlQuery consonantQuery;

        QString consonantCmd = "";
        consonantCmd = QString("SELECT typed FROM dictionary WHERE typed LIKE \"" + keyWord.toLower() + "%\" ");

        int nPos = keyWord.length();
        int i;

        if(consonantQuery.exec(consonantCmd))
        {
            if(consonantQuery.first())
            {
                do
                {
                    consonantList.append(consonantQuery.value(0).toString());
                }while(consonantQuery.next());
            }

            if (!consonantList.isEmpty())
            {
                for(i=0; i < consonantList.length(); i++)
                {
                    if ((keyWord != consonantList.at(i)) && ( currentNextChar != consonantList.at(i).at(nPos)) )
                    {
                        nextCharList.append(consonantList.at(i).at(nPos));
                        currentNextChar = consonantList.at(i).at(nPos);
                    }
                }
                mNextCharList = nextCharList;
            }
        }
    }
    else
    {
        LOG2(QString("setKeyWordConsonants():keyWord is Empty."), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical()<<"[Keypad]setKeyWordConsonants():keyWord is Empty.";
    }

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}


int QwertyKeypadChinesePinyin::getCount()
{
    return m_nPinyinCount;
}

void QwertyKeypadChinesePinyin::addItem(const QString name, const int size)
{
    PinyinListModelItem *pNewItem = new PinyinListModelItem(name, size);
    m_itemList.append(pNewItem);
}

PinyinListModelItem* QwertyKeypadChinesePinyin::get(int index)
{
    if ( index < 0 || index > m_itemList.count() || m_itemList.count() <= 0 )
    {
        //qCritical()<< "something is wrong. Return NULL.";
        LOG2(QString("something is wrong. Return NULL."), Logger::PRIORITY_HI); //added for Log System Lock Issue
        return NULL;
    }

    return m_itemList.at(index);
}

int QwertyKeypadChinesePinyin::getItemSize(int index)
{
    if ( index < 0 || index > m_itemList.count() || m_itemList.count() <= 0 )
    {
        //qCritical()<< "something is wrong. Return NULL.";
        LOG2(QString("something is wrong. Return NULL."), Logger::PRIORITY_HI); //added for Log System Lock Issue
        return NULL;
    }

    return get(index)->getItemSize();
}

QString QwertyKeypadChinesePinyin::getItemName(int index)
{
    if ( index < 0 || index > m_itemList.count() || m_itemList.count() <= 0 )
    {
        //qCritical()<< "something is wrong. Return NULL.";
        LOG2(QString("something is wrong. Return NULL."), Logger::PRIORITY_HI); //added for Log System Lock Issue
        return NULL;
    }

    return get(index)->getItemName();
}

QString QwertyKeypadChinesePinyin::getNameByItemIndexOfPage(int index)
{
    if(mPageList.length())
    {
        if(mPageList.at(m_nCurrentPageNum).length() <= index)
            return "";
        else
            return mPageList.at(m_nCurrentPageNum).at(index);
    }
    else
        return "";
}

QVariant QwertyKeypadChinesePinyin::data( const QModelIndex &index, int role ) const
{
    if(mPageList.length())
    {
        if(mPageList.at(m_nCurrentPageNum).length() <= index.row())
            return QVariant();
    }

    if(role == nameRole)
        return QVariant( mPageList.at(m_nCurrentPageNum).at(index.row()) );

    if(role == sizeRole)
        return QVariant( mPageList.at(m_nCurrentPageNum).at(index.row()).length());

    return QVariant();
}

int QwertyKeypadChinesePinyin::rowCount ( const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(mPageList.length())
        return mPageList.at(m_nCurrentPageNum).length();
    else
        return 0;
    //return mList.length();
}

void QwertyKeypadChinesePinyin :: clearBuffer()
{
    m_nPinyinCount = 0;
    m_nCurrentPageNum = 0;
    m_nPinyinTotalPage = 0;

    if(mList.length() && mPageList.length())
    {
        beginRemoveRows( QModelIndex(), 0, mPageList.at(m_nCurrentPageNum).length() - 1 );
        mPageList.clear();
        mList.clear();

        //add Han
        while(!m_itemList.isEmpty())
        {
            PinyinListModelItem* tmp = m_itemList.takeFirst();
            if(tmp)
                delete tmp;
        }

        endRemoveRows();
    }

    clearInputString();

    if(mNextCharList.length())
    {
        mNextCharList.clear();
    }

    emit pinyinCountChanged();
}
QString QwertyKeypadChinesePinyin::getPhoneticSpell()
{
    prevPhoneticSpell.clear();
    prevPhoneticSpell = phoneticSpell;
    return phoneticSpell;
}
int QwertyKeypadChinesePinyin::getPrevPhoneticSpellLength()
{
    return prevPhoneticSpell.count();
}


void QwertyKeypadChinesePinyin::addPrevPhoneticInputText(QString inputTxt)
{
    //qCritical() <<"[QT] addPrevPhoneticInputText : " << inputTxt;
    //LOG2(QString("[QT] addPrevPhoneticInputText : %1").arg(inputTxt), Logger::PRIORITY_HI); //added for Log System Lock Issue
    //added for ITS 228022 apostrophe twice input issue
    if(prevPhoneticSpell.count() >= 1)
        LOG2(QString("[QT] last Str : %1").arg(prevPhoneticSpell.at(prevPhoneticSpell.count() - 1)), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical() <<"[QT] last Str : " << prevPhoneticSpell.at(prevPhoneticSpell.count() - 1);

    if(prevPhoneticSpell.count() >= 1
            && prevPhoneticSpell.at(prevPhoneticSpell.count() - 1) == QChar('\''))
    {
        if(inputTxt == "'")
        {
            //qCritical() << "[QT] exception for inputTxt";
            //LOG2(QString("[QT] exception for inputTxt"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            return;
        }
    }
    //added for ITS 228022 apostrophe twice input issue

    prevPhoneticSpell.append(inputTxt);
    //qCritical() <<"[QT] addPrevPhoneticInputText : addResult : " << prevPhoneticSpell;
    //LOG2(QString("[QT] addPrevPhoneticInputText : addResult : %1").arg(prevPhoneticSpell), Logger::PRIORITY_HI); //added for Log System Lock Issue

}

//modify for ITS 227416 Pinyin -> ENG -> Pinyin Delete Issue
void QwertyKeypadChinesePinyin::setUpdateInputText(QString inputTxt)
{
    //qCritical() <<"[QT] setUpdateInputText : " << inputTxt;
    //LOG2(QString("[QT] setUpdateInputText : %1").arg(inputTxt), Logger::PRIORITY_HI); //added for Log System Lock Issue
    prevPhoneticSpell.clear();
    prevPhoneticSpell = inputTxt;
    //qCritical() <<"[QT] setUpdateInputText : " << prevPhoneticSpell;
    //LOG2(QString("[QT] setUpdateInputText : %1").arg(prevPhoneticSpell), Logger::PRIORITY_HI); //added for Log System Lock Issue
}
//modify for ITS 227416 Pinyin -> ENG -> Pinyin Delete Issue

void QwertyKeypadChinesePinyin::setPrevPhoneticInputTextByBack()
{
    //qCritical() <<"[QT] removePrevPhoneticInputText : current :  " << prevPhoneticSpell;
    //LOG2(QString("[QT] removePrevPhoneticInputText : current : %1").arg(prevPhoneticSpell), Logger::PRIORITY_HI); //added for Log System Lock Issue
    int spellCount = prevPhoneticSpell.count();
    spellCount--;
    if(spellCount >=0)
    {
        prevPhoneticSpell.chop(1);
    }
    else
    {

    }
    //qCritical() <<"[QT] removePrevPhoneticInputText : changed :  " << prevPhoneticSpell;
    //LOG2(QString("[QT] removePrevPhoneticInputText : changed : %1").arg(prevPhoneticSpell), Logger::PRIORITY_HI); //added for Log System Lock Issue
}

int QwertyKeypadChinesePinyin::getPhoneticCountResult()
{
    //qCritical() << "[QT] getPhoneticCountResult() :: " << phoneticSizeRange ;
    //LOG2(QString("[QT] getPhoneticCountResult() :: %1").arg(phoneticSizeRange), Logger::PRIORITY_HI); //added for Log System Lock Issue
    return phoneticSizeRange;

}
void QwertyKeypadChinesePinyin::clearPhoneticBuffer()
{
    //qCritical() << "[QT] clearPhoneticBuffer() :: ";
    //LOG2(QString("[QT] clearPhoneticBuffer() ::"), Logger::PRIORITY_HI); //added for Log System Lock Issue
    prevPhoneticSpell.clear();
}

bool QwertyKeypadChinesePinyin::getDKBDCandidate(QString inputString)
{
    //qCritical()<<"getDKBDCandidate() : inputString:"<< inputString;
    //LOG2(QString("getDKBDCandidate() : inputString: %1").arg(inputString), Logger::PRIORITY_HI); //added for Log System Lock Issue
    int status = HWERR_SUCCESS;
    int nCandidateNum = 0;
    QString tempCandidate = "";
    unsigned short candWordBuf[MAX_CANDIDATE_BUFFER_SIZE] = {0,};

    /**
     * Description :  문자 하나를 입력 문자열에 추가한다.
     * int DKBD_CNAddChar(unsigned short wChar)
     * Arg0. wChar : 추가할 문자 코드 값, ex) HW_KEY_SpaceBar_Code
     * Ret. int : [표 1]의 값 중 하나를 반환한다.
     */
    for(int i = 0; i<inputString.length(); i++)
    {
        status = DKBD_CNAddChar(inputString.at(i).toAscii());

        if (status == HWERR_SUCCESS) {
            //qCritical()<<"DKBD_CNAddChar() : success";
            LOG2(QString("DKBD_CNAddChar() : success"), Logger::PRIORITY_HI); //added for Log System Lock Issue
        } else {
            //qCritical()<<"DKBD_CNAddChar() : failed!! status:"<<status;
            LOG2(QString("DKBD_CNAddChar() : failed!! status: %1").arg(status), Logger::PRIORITY_HI); //added for Log System Lock Issue
            return false;
        }
    }
    // 입력 문자에 대한 변환이 끝났는지 판단한다.
    // 종료되었을 경우 1을 반환한다.

    // Phonetic group을 얻어온다.
    unsigned short phoneGroupBuf[HWRE_OUT_PHONETIC_BUFSIZE] = {0, };
    int nPhoneGroupNum = 0;
    nPhoneGroupNum = DKBD_CNGetPhoneticGroup(phoneGroupBuf);
    //qCritical() << "DKBD_CNGetPhoneticGroup() : Number of Phonetic group : " << nPhoneGroupNum;
    //LOG2(QString("DKBD_CNGetPhoneticGroup() : Number of Phonetic group : %1").arg(nPhoneGroupNum), Logger::PRIORITY_HI); //added for Log System Lock Issue
    QString tmpStr;
    for(int i = 0; i < nPhoneGroupNum; i++)
    {
        tmpStr.clear();
        tmpStr = QString::fromUtf16(&phoneGroupBuf[i], 1);
        //qCritical() << "DKBD_CNGetPhoneticGroup() : Char[" << i << "]" << ":: " << tmpStr;
        //LOG2(QString("DKBD_CNGetPhoneticGroup() : Char[%1] %2").arg(i).arg(tmpStr), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
    // Phonetic Group의 인덱스를 설정한다.
    // - 범위는 -1부터 group number + 1까지이다.
    // - Group의 인덱스를 사용하지 않을 경우 -1로 설정한다. 단어추천의 결과는 첫번째 인덱스를 선택한 것과 같다.
    //int nSelectPhoneIndex = -1;
    // 후보단어를 얻어온다.
    // buffer에 저장되는 단어들은 NULL로 구분된다.
    // ex [0x4e2d, 0x56fd, 0x0000, 0x8fd9, 0x4e2a, 0x0000....]

    /**
     * Description : 후보 단어를 반환한다.
     * int DKBD_CNGetWordCandidates(int nSelectPhoneIndex, int nStartWordIndex, unsigned short* pwCandWordBuf, int nCandWordBufSize)
     * Arg0. nSelectPhoneIndex : Group의 index, 값의 범위는 -1 부터 group number + 1 까지이다.
     * Arg1. nStartWordIndex : 후보 단어의 index
     * Arg2. pwCandWordBuf : 후보 단어가 저장될 버퍼
     * Arg3. nCandWordBufSize : 후보단어 버퍼 사이즈
     *
     * Ret. int : 후보 문자의 개수 or [표 1. Error Code]의 값 중 하나를 반환한다.
     */
    nCandidateNum = DKBD_CNGetWordCandidates(-1, 0, candWordBuf, MAX_CANDIDATE_BUFFER_SIZE);
    //qCritical()<<"DKBD_CNGetWordCandidates() : Number of candidate: nCandidateNum:"<< nCandidateNum;
    //LOG2(QString("DKBD_CNGetWordCandidates() : Number of candidate: nCandidateNum: %1").arg(nCandidateNum), Logger::PRIORITY_HI); //added for Log System Lock Issue

    for (int i = 0, j = 0; j < nCandidateNum; i++) {
        if (candWordBuf[i] != 0)
        {
            tempCandidate.append(QString::fromUtf16(&candWordBuf[i], 1));
        }
        else
        {
            addItem(tempCandidate, tempCandidate.length());
            mList.append(tempCandidate);
            m_nPinyinCount++;
            tempCandidate = "";
            j++;
        }
    }

    // Phonetic spell 문자를 얻어온다.
    unsigned short outString[HW_MAX_INPUT_SHOW_LEN] = {0,};
    int length = 0;
    length = DKBD_CNGetInputTransResult(outString);
    //qCritical() << "DKBD_CNGetInputTransResult() : length: " << length;
    //LOG2(QString("DKBD_CNGetInputTransResult() : length: %1").arg(length), Logger::PRIORITY_HI); //added for Log System Lock Issue

    phoneticSpell.clear();
    for(int i = 0; i < length; i++)
    {
        tmpStr.clear();
        tmpStr = QString::fromUtf16(&outString[i], 1);
        phoneticSpell.append(QString::fromUtf16(&outString[i], 1));
        //qCritical()<< "DKBD_CNGetInputTransResult() : String ["<< i <<"]" <<"::"<< tmpStr;
        //LOG2(QString("DKBD_CNGetInputTransResult() : String [%1]%2").arg(i).arg(tmpStr), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }

    for(int i = 0; i < phoneticSpell.length() ; i++)
    {
        //qCritical() <<"Phonetic Spell : ["<< i <<"] [" << phoneticSpell.at(i) <<"]";
        //LOG2(QString("Phonetic Spell :  [%1][%2]").arg(i).arg(phoneticSpell.at(i)), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
     // ex : ti'an -> tian , when back key case :
    if(prevPhoneticSpell.count() > phoneticSpell.count())
    {
        phoneticSizeRange = 0;
    }// ex: tia -> ti'a, when back key case : tian -> delete n prev: tia , phonetic : ti'a
    else if(prevPhoneticSpell.count() < phoneticSpell.count())
    {
        phoneticSizeRange = 1;
    }// ex : ti -> ti, when back key case : tia -> delete a prev: ti , phonetic: ti
    else if(prevPhoneticSpell.count() == phoneticSpell.count())
    {
        phoneticSizeRange = 2;
    }


    return true;
}

bool QwertyKeypadChinesePinyin::getDKBDPrediction(QString searchWord)
{
    if(searchWord == "")
        return false;

    int nCandidateNum = 0;
    unsigned short outString[HW_MAX_INPUT_SHOW_LEN] = {0,};
    unsigned short candWordBuf[MAX_CANDIDATE_BUFFER_SIZE] = {0,};
    //added for Prediction space Prediction not showing next prediction
    QString parsSearchWord = searchWord;

    if(searchWord.contains(" "))
    {

        QStringList tmpStrList = searchWord.split(" ");
        if(tmpStrList.count())
        {
            //for(int i = 0; i < tmpStrList.count(); i++)
            //{
            //    qCritical() << "[QT] Prediction Search Word --------- : " << tmpStrList.at(i);
            //}

            if(tmpStrList.at(tmpStrList.count() - 1) != "")
            {
                parsSearchWord.clear();
                parsSearchWord = tmpStrList.at(tmpStrList.count() - 1);
            }
            //qCritical() << "[QT] Parsed Prediction Search Word --------- : "<< parsSearchWord;
            //LOG2(QString("[QT] Parsed Prediction Search Word --------- : %1").arg(parsSearchWord), Logger::PRIORITY_HI); //added for Log System Lock Issue
        }
    }
    //added for Prediction space Prediction not showing next prediction

    for(int i = 0; i < searchWord.length(); i++) {
        //outString[i] = searchWord[i].unicode();
        outString[i] = parsSearchWord[i].unicode(); //added for Prediction space Prediction not showing next prediction
        //qCritical() << "outString[ "<< i << " ]: "<< outString[i];
    }

    nCandidateNum = DKBD_CNGetPredictResult(outString, NULL, candWordBuf, MAX_CANDIDATE_BUFFER_SIZE);
    //qCritical()<<"PredictResult() : Number of candidate:"<< nCandidateNum;
    //LOG2(QString("PredictResult() : Number of candidate: %1").arg(nCandidateNum), Logger::PRIORITY_HI); //added for Log System Lock Issue

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
                mList.append(tempCandidate);
                m_nPinyinCount++;
                tempCandidate = "";
                j++;
            }
        }

        // 화면에 표시
        if(mList.length())
        {
            makeListByPageEx(); // added for prediction word list(3page)

            if(mPageList.length())
            {
                beginInsertRows( QModelIndex(), 0, mPageList.at(m_nCurrentPageNum).length() - 1 );
                endInsertRows();
            }
        }

        emit pinyinCountChanged();

        return true;
    }
    else // 다음 후보군이 존재하지 않을 경우
    {
        clearBuffer();

        return false;
    }
}
// added for prediction word list(3page)
// 전체 후보군 리스트를 페이지별로 분리하여 저장한다.
void QwertyKeypadChinesePinyin::makeListByPageEx()
{
    int sumTotal = 0;
    QStringList tempStringList;

    // 전체 페이지 초기화
    m_nPinyinTotalPage = 0;

    for(int i=0; i < mList.length(); i++)
    {

        // 한 페이지 안에 들어갈 수 있는 사이즈는 최대 8칸(버튼기준)
        sumTotal += getColumnSizeByString(mList.at(i));
        //qCritical() <<"[QT] makeListByPage For loop : sumTotal : ---------------->" << sumTotal;
        // 1. 8칸을 넘었을 경우.
        if(sumTotal > 8)
        {
            //합계 초기화
            sumTotal = 0;
            //qCritical() <<"[QT] makeListByPage: sumTotal > 8 ---------------->";
            // 전체 페이지 증가
            m_nPinyinTotalPage++;

            // 페이지 리스트에 지금까지 추가한 문자열 리스트를 추가 등록한다
            mPageList.append(tempStringList);

            // 문자열 리스트를 초기화
            if(tempStringList.length())
                tempStringList.clear();

            if(m_nPinyinTotalPage == 3)
            {
                m_nPinyinTotalPage = m_nPinyinTotalPage -1;
                //qCritical() <<"[QT] restrict page size 3 -------------->";
                break;
            }

            // 현재 문자열을 저장
            tempStringList.append(mList.at(i));

            // 한 페이지 안에 들어갈 수 있는 사이즈는 최대 8칸(버튼기준)
            sumTotal += getColumnSizeByString(mList.at(i));

            //마지막 항목일 경우 페이지 리스트에 저장
            if(i == mList.length()-1)
                mPageList.append(tempStringList);
        }
        else // 2. 합계가 8칸이거나, 8칸 이내일경우 문자열 리스트에 글자를 추가 등록한다.
        {
            //qCritical() <<"[QT] makeListByPage: else ---------------->";


            if(sumTotal == 8 && m_nPinyinTotalPage == 2)
            {
                //qCritical() <<"[QT] makeListByPage: remove last word at page 3 !!! ";
                LOG2(QString("makeListByPage: remove last word at page 3 !!!"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            }
            else
            {

                tempStringList.append(mList.at(i));
            }
            //마지막 항목일 경우 페이지 리스트에 저장
            if(i == mList.length()-1)
                mPageList.append(tempStringList);
        }
    }

    emit pinyinTotalPageChanged();
}
// added for prediction word list(3page)
// 전체 후보군 리스트를 페이지별로 분리하여 저장한다.
void QwertyKeypadChinesePinyin::makeListByPage()
{
    int sumTotal = 0;
    QStringList tempStringList;

    // 전체 페이지 초기화
    m_nPinyinTotalPage = 0;

    for(int i=0; i < mList.length(); i++)
    {

        // 한 페이지 안에 들어갈 수 있는 사이즈는 최대 8칸(버튼기준)
        sumTotal += getColumnSizeByString(mList.at(i));

        // 1. 8칸을 넘었을 경우.
        if(sumTotal > 8)
        {
            //합계 초기화
            sumTotal = 0;

            // 전체 페이지 증가
            m_nPinyinTotalPage++;

            // 페이지 리스트에 지금까지 추가한 문자열 리스트를 추가 등록한다
            mPageList.append(tempStringList);

            // 문자열 리스트를 초기화
            if(tempStringList.length())
                tempStringList.clear();

            // 현재 문자열을 저장
            tempStringList.append(mList.at(i));

            // 한 페이지 안에 들어갈 수 있는 사이즈는 최대 8칸(버튼기준)
            sumTotal += getColumnSizeByString(mList.at(i));

            //마지막 항목일 경우 페이지 리스트에 저장
            if(i == mList.length()-1)
                mPageList.append(tempStringList);
        }
        else // 2. 합계가 8칸이거나, 8칸 이내일경우 문자열 리스트에 글자를 추가 등록한다.
        {

            tempStringList.append(mList.at(i));

            //마지막 항목일 경우 페이지 리스트에 저장
            if(i == mList.length()-1)
                mPageList.append(tempStringList);
        }
    }

    emit pinyinTotalPageChanged();
}

// 후보군 문자열의 글자수에 따라 리스트뷰 내 몇칸을 차지하는지를 리턴
// 리스트뷰는 총 8칸 이내 표시, 초과시 다음 페이지에서 후보군 제공
int QwertyKeypadChinesePinyin::getColumnSizeByString(QString candidateString)
{
    int stringSize = candidateString.length();

    switch(stringSize)
    {
    case 1: return 1;
    case 2:
    case 3: return 2;
    case 4:
    case 5:
    case 6: return 3;
    case 7:
    case 8:
    case 9: return 4;
    case 10:
    case 11:
    case 12: return 5;
    default: return 0;
    }

    return 0;
}

// 다음 페이지 후보군 리스트를 표시한다. 마지막 페이지일경우 첫 페이지 후보군 리스트를 표시한다.
void QwertyKeypadChinesePinyin::incrementCurrentPage()
{
    if(mList.length() && mPageList.length())
    {
        beginRemoveRows( QModelIndex(), 0, mPageList.at(m_nCurrentPageNum).length() - 1 );
        endRemoveRows();

        if(m_nCurrentPageNum < m_nPinyinTotalPage)
            m_nCurrentPageNum++;
        else
            m_nCurrentPageNum = 0;

        beginInsertRows( QModelIndex(), 0, mPageList.at(m_nCurrentPageNum).length() - 1 );
        endInsertRows();
    }
}

// 이전 페이지 후보군 리스트를 표시한다. 첫 페이지일경우 마지막 페이지 후보군 리스트를 표시한다.
void QwertyKeypadChinesePinyin::decrementCurrentPage()
{
    if(mList.length() && mPageList.length())
    {
        beginRemoveRows( QModelIndex(), 0, mPageList.at(m_nCurrentPageNum).length() - 1 );
        endRemoveRows();

        if(m_nCurrentPageNum > 0)
            m_nCurrentPageNum--;
        else
            m_nCurrentPageNum = mPageList.length()-1;

        beginInsertRows( QModelIndex(), 0, mPageList.at(m_nCurrentPageNum).length() - 1 );
        endInsertRows();
    }
}
