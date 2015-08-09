#ifndef DIRWATCHER_H
#define DIRWATCHER_H

#include <QObject>
#include <QSettings>
#include <QList>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QQueue>
#include <QFileInfo>
#include <QHash>

enum DirType {
AUDIO,
VIDEO,
IMAGE,
DOCUMENT,
ARCHIVE,
CODE,
OTHERS
};




class DirWatcher : public QObject
{
    Q_OBJECT

public:

    explicit DirWatcher(QObject *parent = 0);
    void addFolder(const QString &folderName);
    void setFolders(const QList<QString> &folderList);
    QList<QString> getFolders();
    bool removeFolder(const QString& folderName);
    void startWatch();
    ~DirWatcher();

    bool isSpecialDirectory(QFileInfo& path);


signals:

public slots:
    void contentChanged(QString path);
    void sortDirectory(QString path);

private :
    DirType getType(QFileInfo &path);
    DirType typeFromMime(QString mimeType);
    void move(const QFileInfo &info, DirType type);
QSettings *mSettings;
QStringList *mFolderList;
QFileSystemWatcher *mWatcher;
QQueue<QFileInfo> *mQ;
QHash<DirType, QString> *mDirNames;
QHash<QString, DirType> *mMimeType;

QHash<QString, QString> *mMimesSuffix;
};

#endif // DIRWATCHER_H
