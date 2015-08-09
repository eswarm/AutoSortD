#include "dirwatcher.h"
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDir>
#include <QDebug>
#include <QDirIterator>

//const QString directories[] = {"audio", "video", "code", "documents", "archives", "images", "others" };
int fileTypePriority[] = {5, 5, 1, 5, 5 ,20, 1};

DirWatcher::DirWatcher(QObject *parent) : QObject(parent)
{

mSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope,"eswar", "smart folder");
mFolderList = new QStringList();
mWatcher = new QFileSystemWatcher();
mDirNames = new QHash<DirType, QString>();
mMimeType = new QHash<QString, DirType>();
connect(mWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(contentChanged(QString)) );

/* Initialize the names */
mDirNames->insert(AUDIO, "audio");
mDirNames->insert(VIDEO, "video");
mDirNames->insert(CODE, "code");
mDirNames->insert(DOCUMENT, "documents");
mDirNames->insert(ARCHIVE, "archives");
mDirNames->insert(OTHERS, "others");

/*Initialize mimes */
mMimeType->insert("audio", AUDIO);
mMimeType->insert("video", VIDEO);

mMimesSuffix = new QHash<QString, QString>();

}

void DirWatcher::addFolder(const QString &folderName)
{

    int size = mSettings->beginReadArray("folders");
    mSettings->endArray();

    mSettings->beginWriteArray("folders");
    mSettings->setArrayIndex(size);
    mSettings->setValue("folderPath",folderName);
    mSettings->endArray();

}

void DirWatcher::setFolders(const QList<QString> &folderList)
{
    mSettings->remove("folders");
    mSettings->beginWriteArray("folders");
    for (int i = 0; i < folderList.size(); ++i) {
        mSettings->setArrayIndex(i);
        mSettings->setValue("folderPath", folderList.at(i));
    }
    mSettings->endArray();
}

QList<QString> DirWatcher::getFolders()
{
    QList<QString> folderList;
    int size = mSettings->beginReadArray("folders");

    for (int i = 0; i < size; ++i) {
        mSettings->setArrayIndex(i);
        QString path = mSettings->value("folderPath").toString();
        folderList.append(path);
    }
    mSettings->endArray();
    return folderList;
}

bool DirWatcher::removeFolder(const QString& folderName)
{
    QList<QString> folderList = getFolders();
    QList<QString> newList;
    bool found = false;

    // Remove the current folder
    for(int i=0 ; i<folderList.size(); i++)
    {
        QString crntFolder = folderList.at(i);
        if(folderName.compare(crntFolder) == 0)
        {
            found = true;
        }
        else
        {
            newList.append(crntFolder);
        }
    }

    // Set the new list.
    setFolders(newList);
    return found;
}

DirWatcher::~DirWatcher()
{
    delete mSettings;
    delete mFolderList;
}

void DirWatcher::startWatch()
{
    mFolderList->clear();
    *mFolderList = getFolders();
    mWatcher->addPaths(*mFolderList);
}

void DirWatcher::contentChanged(QString path)
{

}

void DirWatcher::sortDirectory(QString path)
{
    mMimesSuffix->clear();

    QDir dir(path);

    if(!dir.exists())
    {
        qDebug() << "Directory does not exist";
        return;
    }


    QDirIterator it(path, QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Writable);

    while (it.hasNext()) {
        QString path = it.next();
        qDebug() << path;
        QFileInfo fileInfo = it.fileInfo();
        if(!isSpecialDirectory(fileInfo))
        {
            DirType type = getType(it.fileInfo());
            move(fileInfo, type);
        }
    }

    /*
    foreach (const QString &str, mMimesSuffix->keys())
        qDebug() << str << ":" << mMimesSuffix->value(str);
    */
}

void DirWatcher::move(const QFileInfo &info, DirType type)
{
    qDebug() << "Type :: " << type << "Name :: " << info.fileName();
    if(info.isFile())
    {

    }
    else if(info.isDir())
    {

    }
}

bool DirWatcher::isSpecialDirectory(QFileInfo& path)
{
    if(path.isDir() && mDirNames->values().contains(path.fileName()))
    {
        return true;
    }
    return false;
}


DirType DirWatcher::getType(QFileInfo &path)
{
    QMimeType mimeType;
    QMimeDatabase mimeDatabase;

    if(path.isFile())
    {
       mimeType = mimeDatabase.mimeTypeForFile(path);

       mMimesSuffix->insert(path.suffix(), mimeType.name());
       //qDebug() << "Suffix " << path.suffix() << " mime " << mimeType;

       if(mimeType.isValid())
       {
            QString type = mimeType.name();
            return typeFromMime(type);
       }
       else
       {
          return OTHERS;
       }
    }
    else if(path.isDir())
    {
        QDirIterator it(path.absoluteFilePath(), QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Writable, QDirIterator::Subdirectories);

        QHash<DirType, int> fileTypes;

        for(int i=AUDIO; i<=OTHERS; i++)
            fileTypes.insert((DirType)i, 0);

        while (it.hasNext()) {
            it.next();
            QFileInfo fileInfo = it.fileInfo();
            if( fileInfo.isFile() )
            {
                DirType fileType = getType(fileInfo);
                fileTypes.insert(fileType, fileTypes.value(fileType) + 1 );
            }
        }

        for(int i=AUDIO; i<=OTHERS; i++)
            fileTypes.insert((DirType)i, fileTypes.value((DirType)i) * fileTypePriority[i]);

        long max = 0;
        DirType fileType = OTHERS;
        for(int i=AUDIO; i<=OTHERS; i++)
        {
            if(fileTypes.value((DirType)i) > max)
            {
                fileType = (DirType)i;
                max = fileTypes.value(fileType);
            }
        }
        return fileType;
    }

    return OTHERS;
}

DirType DirWatcher::typeFromMime(QString mimeType)
{

    QString main, sub;
    // TODO :: figure out the type from the file.
    if(mimeType.indexOf("/") != -1)
    {
        QStringList mime = mimeType.split('/');
        main = mime.at(0);
        sub = mime.at(1);
    }

    if(main == "image")
    {
        return IMAGE;
    }
    else if(main == "video")
    {
        return VIDEO;
    }
    else if(main == "audio")
    {
        return AUDIO;
    }
    else if(
            mimeType == "application/x-ms-dos-executable" ||
            mimeType == "application/zip" ||
            mimeType == "application/x-rar" ||
            mimeType == "application/x-cd-image" ||
            mimeType == "application/x-7z-compressed" ||
            mimeType == "application/zip" ||
            mimeType == "application/x-msi" ||
            mimeType == "application/x-compressed-tar" ||
            mimeType == "application/x-tar"
            )
    {
        return ARCHIVE;
    }
    else if(
            mimeType == "application/pdf" ||
            mimeType == "text/plain" ||
            mimeType == "application/rtf" ||
            mimeType == "application/msword" ||
            mimeType == "text/csv"
            )
    {
        return DOCUMENT;
    }
    else if(
            mimeType == "text/x-java" ||
            mimeType == "text/x-python" ||
            mimeType == "text/x-chdr" ||
            mimeType == "text/html" ||
            mimeType == "text/x-qml" ||
            mimeType == "text/x-makefile" ||
            mimeType == "text/x-csrc" ||
            mimeType == "text/x-cmake" ||
            mimeType == "text/css" ||
            mimeType == "text/x-c++src" ||
            mimeType == "text/x-c++hdr"
            )
    {
        return CODE;
    }

    return OTHERS;

}


