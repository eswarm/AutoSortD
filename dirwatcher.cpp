#include "dirwatcher.h"
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include "androidfiledialog.h"

//const QString directories[] = {"audio", "video", "code", "documents", "archives", "images", "others" };
int fileTypePriority[] = {5, 5, 1, 5, 5 ,10, 1};
DirWatcher* DirWatcher::mSelf;

DirWatcher::DirWatcher(QObject *parent) : QObject(parent)
{

    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationDirPath());
    mSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope,"eswarm.in", "AutoSortD");

    /* Get the folder list from settings */
    mFolderList = new QList<QVariant>();
    *mFolderList = getFolders();
    mWatcher = new QFileSystemWatcher();
    mDirNames = new QHash<DirType, QString>();

    connect(mWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(contentChanged(QString)) );

    /* Initialize the names */
    mDirNames->insert(AUDIO, "audio");
    mDirNames->insert(VIDEO, "video");
    mDirNames->insert(CODE, "code");
    mDirNames->insert(DOCUMENT, "documents");
    mDirNames->insert(ARCHIVE, "archives");
    mDirNames->insert(OTHERS, "others");
    mDirNames->insert(IMAGE, "images");

    //mMimesSuffix = new QHash<QString, QString>();

    // If this is first launch, add downloads directory.

    //qDebug() << QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);

    if( isFirstLaunch() )
    {
        QStringList appList = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
        for(int i=0; i<appList.size(); i++)
        {
        addToSettings(appList.at(i));
        }
        //addToSettings(QStandardPaths::displayName(QStandardPaths::DownloadLocation));
    }

    startWatch();
    connect(&mFutureWatcher, SIGNAL(finished()), this, SLOT(sortFinished()));
}

DirWatcher* DirWatcher::getInstance(QObject *parent)
{
    if(mSelf == NULL)
    {
        mSelf = new DirWatcher(parent);
    }
    return mSelf;
}

void DirWatcher::addToSettings(const QString &folderName)
{

    int size = mSettings->beginReadArray("folders");
    mSettings->endArray();
    mSettings->beginWriteArray("folders");
    mSettings->setArrayIndex(size);
    mSettings->setValue("folderPath",folderName);
    mSettings->endArray();

    emit textChanged(" Added " + folderName);
}

void DirWatcher::setFolders(const QList<QVariant> &folderList)
{
    mSettings->remove("folders");
    mSettings->beginWriteArray("folders");
    for (int i = 0; i < folderList.size(); ++i) {
        mSettings->setArrayIndex(i);
        mSettings->setValue("folderPath", folderList.at(i));
    }
    mSettings->endArray();
}

QList<QVariant> DirWatcher::getFolders()
{
    qDebug() << "Getfolders";
    QList<QVariant> folderList;
    int size = mSettings->beginReadArray("folders");

    for (int i = 0; i < size; ++i) {
        mSettings->setArrayIndex(i);
        QVariant path = mSettings->value("folderPath");
        folderList.append(path);
    }
    mSettings->endArray();
    qDebug() << "Getfolders end";
    return folderList;
}

bool DirWatcher::removeFromSettings(const QString& folderName)
{
    QList<QVariant> folderList = getFolders();
    QList<QVariant> newList;
    bool found = false;

    // Remove the current folder
    for(int i=0 ; i<folderList.size(); i++)
    {
        QVariant crntFolder = folderList.at(i);
        if(folderName.compare(crntFolder.toString()) == 0)
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

    emit textChanged(" Removed " + folderName);
    return found;
}

DirWatcher::~DirWatcher()
{
    delete mSettings;
    delete mFolderList;
    delete mWatcher;
    delete mQ;
    delete mDirNames;
    //delete mMimesSuffix;
}

void DirWatcher::startWatch()
{
    mFolderList->clear();
    *mFolderList = getFolders();
    QVariant v;
    foreach(v, *mFolderList)
    {
        mWatcher->addPath(v.toString());
    }
}

void DirWatcher::clearWatch()
{
    mFolderList->clear();
    *mFolderList = getFolders();
    QVariant v;
    foreach(v, *mFolderList)
    {
        mWatcher->removePath(v.toString());
    }
}

void DirWatcher::sortFinished()
{
    startWatch();
}

void DirWatcher::contentChanged(QString path)
{
    qDebug() << "Content changed at " << path;
    startSort();
}


void DirWatcher::startSort()
{
    QMutexLocker locker(&mutex);
    clearWatch();
    QFuture<void> future = QtConcurrent::run(this, &DirWatcher::sortAll);
    mFutureWatcher.setFuture(future);
}

void DirWatcher::sortAll()
{
    mFolderList->clear();
    *mFolderList = getFolders();
    QVariant v;
    foreach(v, *mFolderList)
    {
        sortDirectory(v.toString());
    }
}

void DirWatcher::sortDirectory(QString path)
{
    QDir dir(path);

    if(!dir.exists())
    {
        qDebug() << "Directory does not exist";
        return;
    }

    QDirIterator it(path, QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Writable);

    while (it.hasNext()) {
        QString crntPath = it.next();
        //qDebug() << crntPath;
        QFileInfo fileInfo = it.fileInfo();
        if(!isSpecialDirectory(fileInfo))
        {
            DirType type = getType(fileInfo);
            move(fileInfo, type, path);
        }
    }
    return;
}

void DirWatcher::move(const QFileInfo &info, DirType type, const QString& path)
{
    qDebug() << "Type :: " << getStringType(type) << "Name :: " << info.fileName();
    bool status = false;
    QString change;
    if(info.isFile())
    {
        QFile file;
        QDir dir(path);
        QString dirName = mDirNames->value(type);
        dir.mkdir(dirName);
        QString newPath = dir.filePath(dirName + "/" + info.fileName());
        change = info.absoluteFilePath() + "\t-->\t" + newPath ;
        status = file.rename(info.absoluteFilePath(), newPath);
    }
    else if(info.isDir())
    {
        QDir dir(info.absoluteFilePath());
        QString dirName = dir.dirName();
        dir.setPath(path);
        QString dirTypeName = mDirNames->value(type);
        dir.mkdir(dirTypeName);
        QString newPath = dir.filePath(dirTypeName + "/" + dirName);
        change = info.absoluteFilePath() + "\t-->\t" + newPath ;
        status = dir.rename(info.absoluteFilePath(), newPath);
    }

    if(status == false)
    {
        change = "unable to move " +info.absoluteFilePath();
    }

    emit textChanged(change + "\n");
    return;
}


bool DirWatcher::isSpecialDirectory(QFileInfo& path)
{
    if(path.isDir() && mDirNames->values().contains(path.fileName()))
    {
        return true;
    }
    return false;
}


DirType DirWatcher::getType(QFileInfo& path)
{
    QMimeType mimeType;
    QMimeDatabase mimeDatabase;

    if(path.isFile())
    {
        mimeType = mimeDatabase.mimeTypeForFile(path);

        //mMimesSuffix->insert(path.suffix(), mimeType.name());
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

QString DirWatcher::getStringType(DirType dirType)
{
    switch(dirType)
    {
    case AUDIO :
        return "audio";
    case VIDEO:
        return "video";
    case IMAGE:
        return "image";
    case DOCUMENT:
        return "document";
    case ARCHIVE:
        return "archive";
    case CODE:
        return "code";
    case OTHERS:
        return "others";
    default:
        return "not a valid type";
    }
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
            mimeType == "application/x-tar" ||
            mimeType == "application/x-java-archive" ||
            mimeType == "application/x-sharedlib" ||
            mimeType == "application/octet-stream" ||
            mimeType == "application/vnd.android.package-archive" ||
            mimeType == "application/x-bzip-compressed-tar"
            )
    {
        return ARCHIVE;
    }
    else if(
            mimeType == "application/pdf" ||
            mimeType == "text/plain" ||
            mimeType == "application/rtf" ||
            mimeType == "application/msword" ||
            mimeType == "text/csv" ||
            mimeType == "application/xml"
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
            mimeType == "text/x-c++hdr" ||
            mimeType == "text/x-scala" ||
            mimeType == "application/javascript" ||
            mimeType == "application/x-shellscript" ||
            mimeType == "text/x-matlab" ||
            mimeType == "application/x-perl" ||
            mimeType == "text/x-objcsrc" ||
            mimeType == "text/x-verilog" ||
            mimeType == "text/x-markdown"
            )
    {
        return CODE;
    }

    //qDebug() << "unknown mimetype :: " << mimeType;
    return OTHERS;

}

bool DirWatcher::isFirstLaunch()
{
    bool firstLaunch = mSettings->value("firstLaunch", true).toBool();
    mSettings->setValue("firstLaunch", false);
    return firstLaunch;
}

// singleton type provider function (callback)
QObject *DirWatcher::singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    DirWatcher *dirWatcher = DirWatcher::getInstance(NULL);
    return dirWatcher;
}


void DirWatcher::launchFileDialog() {
    AndroidFileDialog *fileDialog = new AndroidFileDialog();
    connect(fileDialog, SIGNAL(existingFileNameReady(QString)), this, SLOT(openFileNameReady(QString)));
    bool success = fileDialog->provideExistingFileName();
    if (!success) {
        qDebug() << "Problem with JNI or sth like that...";
        disconnect(fileDialog, SIGNAL(existingFileNameReady(QString)), this, SLOT(openFileNameReady(QString)));
        //or just delete fileDialog instead of disconnect
    }
}

void DirWatcher::openFileNameReady(QString fileName)
{
    if (!fileName.isNull()) {
            emit fileSelected(fileName);
        } else {
            qDebug() << "User did not choose file";
        }
}
