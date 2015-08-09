#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dirwatcher.h"
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void directorySelected(QString path);

private:
    Ui::MainWindow *ui;

    DirWatcher *mDirWatcher;
    QFileDialog *mFileDialog;
};

#endif // MAINWINDOW_H
