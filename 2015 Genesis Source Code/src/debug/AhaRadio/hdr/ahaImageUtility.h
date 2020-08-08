#ifndef AHAIMAGEUTILITY_H
#define AHAIMAGEUTILITY_H

#include "ahapredefinedvalues.h"
void clearPresetLogoDirectory();

void clearStationLogoDirectory();
void clearContentImagesDirectory();
void clearPartnerLogoDirectory();
void clearAlbumArtDirectory();

int saveImageToFile(bool bCleanDirectory, QByteArray &byteArray, QString &strImageURL, QString &filePath);

QString generateFileName(QString &strImageURL);
#endif // AHAIMAGEUTILITY_H
