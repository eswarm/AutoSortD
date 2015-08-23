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
#include <QQmlListProperty>
#include <QQmlEngine>
#include <QJSEngine>
#include <QMutex>
#include <QMutexLocker>

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

    Q_INVOKABLE void addToSettings(const QString &folderName);
    void setFolders(const QList<QVariant> &folderList);
    Q_INVOKABLE QList<QVariant> getFolders();
    Q_INVOKABLE bool removeFromSettings(const QString& folderName);
    Q_INVOKABLE void startWatch();
    Q_INVOKABLE void clearWatch();
    ~DirWatcher();
    bool isSpecialDirectory(QFileInfo& path);
    static DirWatcher* getInstance(QObject *parent);
    bool isFirstLaunch();
    static QObject *singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE void startSort();


signals:
    void textChanged(QString msg);
    void foldersChanged(QList<QString> folders);


public slots:
    void contentChanged(QString path);
    void sortDirectory(QString path);
    void sortFinished();

private :

    void sortAll();
    explicit DirWatcher(QObject *parent = 0);
    DirType getType(QFileInfo& path);
    DirType typeFromMime(QString mimeType);
    void move(const QFileInfo &info, DirType type, const QString& path);
    QString getStringType(DirType dirType);

QSettings *mSettings;
QList<QVariant> *mFolderList;
QFileSystemWatcher *mWatcher;
QQueue<QFileInfo> *mQ;
QHash<DirType, QString> *mDirNames;
static DirWatcher *mSelf;
QMutex mutex;
QFutureWatcher mFutureWatcher;
};

#endif // DIRWATCHER_H
