#include "ahaImageUtility.h"
#include <QCryptographicHash>

void createOrCleanDirectory(bool bCleanDirectory, QString &directoryPath);


void clearStationLogoDirectory()
{
    QString filePath;
    filePath.append(AHA_STATION_LOGO_PATH);
    createOrCleanDirectory(true, filePath);
}

void clearContentImagesDirectory()
{
    QString filePath;
    filePath.append(AHA_CONTENT_IMAGES_PATH);
    createOrCleanDirectory(true, filePath);
}

void clearPartnerLogoDirectory()
{
    QString filePath;
    filePath.append(AHA_PARTNER_LOGO_PATH);
    createOrCleanDirectory(true, filePath);
}

void clearAlbumArtDirectory()
{
    QString filePath;
    filePath.append(AHA_ALBUM_ART_PATH);
    createOrCleanDirectory(true, filePath);
}

void clearPresetLogoDirectory()
{
    QString filePath;
    filePath.append(AHA_STATION_PRESET_PATH);
    createOrCleanDirectory(true, filePath);
}

void createOrCleanDirectory(bool bCleanDir, QString &directoryPath)
{
    QDir albumArtDir(directoryPath);
    if(!albumArtDir.exists())
    {
        albumArtDir.mkpath(directoryPath);
    }
    else
    {
        //Check if any Album Art file is present and delete them
        if(bCleanDir)
        {
            for(uint i= 0; i < albumArtDir.count();i++)
            {
                albumArtDir.remove(albumArtDir.entryList().at(i));
            }
        }
    }
}

QString generateFileName(QString &strImageURL)
{
    QString strExtension;
    QString strPNGExtension("png");
    QFileInfo fileInfo;
    QString strUniqueFileName;

    strUniqueFileName = QString(QCryptographicHash::hash(strImageURL.toUtf8().data(),QCryptographicHash::Md5).toHex());

    fileInfo = strImageURL;
    strExtension = fileInfo.suffix();
    if(strExtension.compare(strPNGExtension, Qt::CaseInsensitive) == 0)
        strUniqueFileName.append(".png");
    else
        strUniqueFileName.append(".jpg");

    return strUniqueFileName;
}

int saveImageToFile(bool bCleanDirectory, QByteArray &byteArray, QString &strImageURL, QString &filePath)
{
    QFile imageFile;
    QString strUniqueFileName;

    createOrCleanDirectory(bCleanDirectory, filePath);

    if(strImageURL.length() == 0)
    {
        return 0;
    }

    strUniqueFileName = generateFileName(strImageURL);
    filePath.append(strUniqueFileName);

    imageFile.setFileName(filePath);

    if(imageFile.open(QIODevice::WriteOnly))
    {
        imageFile.write(byteArray);
        imageFile.close();
        return 1;
    }
    else
    {
        DEBUG_STATEHANDLER("Error opening the file");
        return 0;
    }
}
