#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QPair>
#include <QMessageBox>
#include "readsps.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}







