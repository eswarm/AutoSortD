#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mDirWatcher = new DirWatcher();

    mFileDialog = new QFileDialog(this);
    mFileDialog->setFileMode(QFileDialog::Directory);

    connect(ui->addFolderButton, SIGNAL(clicked(bool)), mFileDialog, SLOT(show()));
    connect(mFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(directorySelected(QString)));

    mDirWatcher->sortDirectory("C:/Users/Eswar/Downloads");

}

MainWindow::~MainWindow()
{
    delete mDirWatcher;
    delete mFileDialog;
    delete ui;
}

void MainWindow::directorySelected(QString path)
{
    mDirWatcher->sortDirectory(path);
}
