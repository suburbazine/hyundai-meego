#ifndef QWERTY_KEYPAD_CHINESE_PINYIN_H
#define QWERTY_KEYPAD_CHINESE_PINYIN_H

#include <QDeclarativeItem>
#include <DHAVN_QwertyKeypad_ChinesePinyin_Item.h>
#include <QtSql>

class PinyinListModelItem;
class QwertyKeypadChinesePinyin: public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Layout)
public:
    enum Layout{
        LayoutMandarinPinyin,
        LayoutMandarinHandWriting,
        LayoutCantonesePinyin,
        LayoutCantoneseHandWriting,
        LayoutMax
    };

    explicit QwertyKeypadChinesePinyin(QObject *parent = 0 );
    virtual ~QwertyKeypadChinesePinyin();

    //Han
    Q_INVOKABLE PinyinListModelItem* get(int index);
    Q_INVOKABLE int getItemSize(int index);
    Q_INVOKABLE QString getItemName(int index);
    void addItem(QString name, int size);

    Q_INVOKABLE QString getNameByItemIndexOfPage(int index);

    Q_INVOKABLE void  setKeypadType(int nType, bool bPhoneBook, QString phoneBookDBPath);
    Q_INVOKABLE void  setKeyWord(QString keyWord);
    Q_INVOKABLE void  clearBuffer(); // By Han.
    Q_INVOKABLE int  getCount();
    Q_INVOKABLE QStringList  getConsonantsNextCharList();
    void  setKeyWordPhoneBook(QString keyWord);
    void setKeyWordConsonants(QString keyWord);

    QVariant data( const QModelIndex &index, int role ) const;
    int rowCount ( const QModelIndex &parent = QModelIndex() ) const;


    Q_PROPERTY(int pinyinCount READ getPinyinCount NOTIFY pinyinCountChanged)

    Q_PROPERTY(int currentPageNum READ getCurrentPageNum NOTIFY currentPageNumChanged)
    Q_PROPERTY(int pinyinTotalPage READ getPinyinTotalPage NOTIFY pinyinTotalPageChanged)

    int getPinyinCount(){ return m_nPinyinCount; }
    int getCurrentPageNum(){ return m_nCurrentPageNum; }
    int getPinyinTotalPage(){ return m_nPinyinTotalPage; }

    Q_INVOKABLE bool getDKBDPrediction(QString searchWord);

    Q_INVOKABLE void incrementCurrentPage();
    Q_INVOKABLE void decrementCurrentPage();

    QString phoneticSpell; //added for modify Pinyin result word
    QString prevPhoneticSpell; //added for modify Pinyin result word
    Q_INVOKABLE QString getPhoneticSpell(); //added for modify Pinyin result word
    Q_INVOKABLE int getPrevPhoneticSpellLength();//added for modify Pinyin result word
    Q_INVOKABLE void addPrevPhoneticInputText(QString inputTxt);//added for modify Pinyin result word
    Q_INVOKABLE void setUpdateInputText(QString inputTxt); //added for modify Pinyin result word //modify for ITS 227416 Pinyin -> ENG -> Pinyin Delete Issue
    Q_INVOKABLE void setPrevPhoneticInputTextByBack();//added for modify Pinyin result word

    Q_INVOKABLE int getPhoneticCountResult();//added for modify Pinyin result word
    Q_INVOKABLE void clearPhoneticBuffer(); //added for modify Pinyin result word
    int phoneticSizeRange; //added for modify Pinyin result word
                           // ( 0 : prev > current , 1: prev < current, 2: prev == current )
private:
    void connectMandarinDB();
    void connectBTPhoneBookDB(bool bPhoneBook, QString phoneBookDBPath);
    void createKeyboardEngine();
    void clearInputString();
    void destroyKeyboardEngine();
    void setDKBDLibrary();

    bool getDKBDCandidate(QString inputString);

    int getColumnSizeByString(QString candidateString);
    void makeListByPage();
    void makeListByPageEx(); // added for prediction word list(3page)

protected:
    enum CListModelRoles
    {
        nameRole = Qt::UserRole + 1,
        sizeRole,
        maxRole
    };
signals:
    void pinyinCountChanged();
    void phoneBookDBOpenFailed();

    void currentPageNumChanged();
    void pinyinTotalPageChanged();

private:
    QList<QStringList> mPageList;
    QStringList     mList;
    QSqlDatabase    mDB;
    int             mKeypadType;
    int             m_nPinyinCount;

    int             m_nCurrentPageNum;
    int             m_nPinyinTotalPage;

    QStringList mNextCharList;

    bool            m_bCreatedDKBD;
    bool            m_bUsedPhoneBookDB;
    QString         m_PhoneBookDBPath;


    QList<PinyinListModelItem*> m_itemList;

};

#endif // CANDIDATE_INPUT_H
