#include "DABDataFrequencyTable.h"

bool DABDataFrequencyTable::isLoaded = false;
QHash <int, QString> DABDataFrequencyTable::m_hashTableKoreaBand;
QHash <int, QString> DABDataFrequencyTable::m_hashTableEuropeBandIII;
QHash <int, QString> DABDataFrequencyTable::m_hashTableEuropeLBand;

DABDataFrequencyTable::DABDataFrequencyTable(QObject *parent) :
    QObject(parent)
{

}

bool
DABDataFrequencyTable::LoadData(void)
{
    // Korea Band(BAND-III)
    m_hashTableKoreaBand[175280] = "7A";
    m_hashTableKoreaBand[177008] = "7B";
    m_hashTableKoreaBand[178736] = "7C";
    m_hashTableKoreaBand[181280] = "8A";
    m_hashTableKoreaBand[183008] = "8B";
    m_hashTableKoreaBand[184736] = "8C";
    m_hashTableKoreaBand[187280] = "9A";
    m_hashTableKoreaBand[189008] = "9B";
    m_hashTableKoreaBand[190736] = "9C";
    m_hashTableKoreaBand[193280] = "10A";
    m_hashTableKoreaBand[195008] = "10B";
    m_hashTableKoreaBand[196736] = "10C";
    m_hashTableKoreaBand[199280] = "11A";
    m_hashTableKoreaBand[201008] = "11B";
    m_hashTableKoreaBand[202736] = "11C";
    m_hashTableKoreaBand[205280] = "12A";
    m_hashTableKoreaBand[207008] = "12B";
    m_hashTableKoreaBand[208736] = "12C";
    m_hashTableKoreaBand[211280] = "13A";
    m_hashTableKoreaBand[213008] = "13B";
    m_hashTableKoreaBand[214736] = "13C";

    m_hashTableEuropeBandIII[174928] = "5A";
    m_hashTableEuropeBandIII[176640] = "5B";
    m_hashTableEuropeBandIII[178352] = "5C";
    m_hashTableEuropeBandIII[180064] = "5D";
    m_hashTableEuropeBandIII[181936] = "6A";
    m_hashTableEuropeBandIII[183648] = "6B";
    m_hashTableEuropeBandIII[185360] = "6C";
    m_hashTableEuropeBandIII[187072] = "6D";
    m_hashTableEuropeBandIII[188928] = "7A";
    m_hashTableEuropeBandIII[190640] = "7B";
    m_hashTableEuropeBandIII[192352] = "7C";
    m_hashTableEuropeBandIII[194064] = "7D";
    m_hashTableEuropeBandIII[195936] = "8A"; //20120423
    m_hashTableEuropeBandIII[197648] = "8B";
    m_hashTableEuropeBandIII[199360] = "8C";
    m_hashTableEuropeBandIII[201072] = "8D";
    m_hashTableEuropeBandIII[202928] = "9A";
    m_hashTableEuropeBandIII[204640] = "9B";
    m_hashTableEuropeBandIII[206352] = "9C";
    m_hashTableEuropeBandIII[208064] = "9D"; //208264
    m_hashTableEuropeBandIII[209936] = "10A";
    m_hashTableEuropeBandIII[211648] = "10B";
    m_hashTableEuropeBandIII[213360] = "10C";
    m_hashTableEuropeBandIII[215072] = "10D";
    m_hashTableEuropeBandIII[210096] = "10N";
    m_hashTableEuropeBandIII[216928] = "11A";
    m_hashTableEuropeBandIII[218640] = "11B";
    m_hashTableEuropeBandIII[220352] = "11C";
    m_hashTableEuropeBandIII[222064] = "11D";
//    m_hashTableEuropeBandIII[217088] = "11N";
    m_hashTableEuropeBandIII[223936] = "12A";
    m_hashTableEuropeBandIII[225648] = "12B";
    m_hashTableEuropeBandIII[227360] = "12C";
    m_hashTableEuropeBandIII[229072] = "12D";
    m_hashTableEuropeBandIII[224096] = "12N";
    m_hashTableEuropeBandIII[230784] = "13A";
    m_hashTableEuropeBandIII[232496] = "13B";
    m_hashTableEuropeBandIII[234208] = "13C";
    m_hashTableEuropeBandIII[235776] = "13D";
    m_hashTableEuropeBandIII[237488] = "13E";
    m_hashTableEuropeBandIII[239200] = "13F";

    m_hashTableEuropeLBand[1452960] = "LA";
    m_hashTableEuropeLBand[1454672] = "LB";
    m_hashTableEuropeLBand[1456384] = "LC";
    m_hashTableEuropeLBand[1458096] = "LD";
    m_hashTableEuropeLBand[1459808] = "LE";
    m_hashTableEuropeLBand[1461520] = "LF";
    m_hashTableEuropeLBand[1463232] = "LG";
    m_hashTableEuropeLBand[1464944] = "LH";
    m_hashTableEuropeLBand[1466656] = "LI";
    m_hashTableEuropeLBand[1468368] = "LJ";
    m_hashTableEuropeLBand[1470080] = "LK";
    m_hashTableEuropeLBand[1471792] = "LL";
    m_hashTableEuropeLBand[1473504] = "LM";
    m_hashTableEuropeLBand[1475216] = "LN";
    m_hashTableEuropeLBand[1476928] = "LO";
    m_hashTableEuropeLBand[1478640] = "LP";
    m_hashTableEuropeLBand[1480352] = "LQ";
    m_hashTableEuropeLBand[1482064] = "LR";
    m_hashTableEuropeLBand[1483776] = "LS";
    m_hashTableEuropeLBand[1485488] = "LT";
    m_hashTableEuropeLBand[1487200] = "LU";
    m_hashTableEuropeLBand[1488912] = "LV";
    m_hashTableEuropeLBand[1490624] = "LW";

    return true;
}

QString
DABDataFrequencyTable::GetLabel(int freq)
{
    if(!isLoaded)
        LoadData();

    QString label = m_hashTableKoreaBand.value(freq, "");
    if(label != "")
        return label;

    label = m_hashTableEuropeBandIII.value(freq, "");
    if(label != "")
        return label;

    label = m_hashTableEuropeLBand.value(freq, "");
    if(label != "")
        return label;

    return "-";
}

