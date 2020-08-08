#ifndef DABDATAFREQUENCYTABLE_H
#define DABDATAFREQUENCYTABLE_H

#include <QObject>
#include <QHash>

class DABDataFrequencyTable : public QObject
{
    Q_OBJECT
public:
    explicit DABDataFrequencyTable(QObject *parent = 0);
    static QString GetLabel(int freq);
signals:

public slots:

private:
    static bool isLoaded;
    static bool LoadData(void);
    static QHash <int, QString> m_hashTableKoreaBand;
    static QHash <int, QString> m_hashTableEuropeBandIII;
    static QHash <int, QString> m_hashTableEuropeLBand;

};

#endif // DABDATAFREQUENCYTABLE_H
