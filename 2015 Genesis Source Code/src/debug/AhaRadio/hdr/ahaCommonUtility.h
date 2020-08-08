#ifndef AHACOMMONUTILITY_H
#define AHACOMMONUTILITY_H

#include <QObject>
#include <QDataStream>
#include <stdint.h>
#include <stdio.h>
#include "ahapredefinedvalues.h"
#include <DHAVN_IPodAppController_Def.h>


uint16_t generateUniqueQueryID(bool bGenerateNextID);
uint16_t calculateCheckSum(QByteArray &pInData);
void printBinaryByteArray(QByteArray array);
uint16_t byteToUint16_t(QByteArray &byteArray, int nIndex);
uint64_t byteToUint64_t(QByteArray &byteArray, int nIndex);
int64_t byteToInt64_t(QByteArray &byteArray, int nIndex);
double byteToDouble(QByteArray &byteArray, int nIndex);
uint32_t byteToUint32_t(QByteArray &byteArray, int nIndex);
QString byteToQString(QByteArray &byteArray, int nIndex, int nByteLength);

bool doubleIsZero(double dbValue);


#endif // AHACOMMONUTILITY_H
