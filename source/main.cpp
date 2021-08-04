#include "mainwindow.h"

#include <QApplication>

#include <QFile>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Установка глобального стиля приложения (использую для отображения QTableWidget)
    //QApplication::setStyle(new myStyle);

    // Установка css стиля приложения
    QFile styleFile(":/qss/resources/qss/style.qss");
    styleFile.open(QFile::ReadOnly);
    a.setStyleSheet(styleFile.readAll());

    MainWindow w;

    // Настройки отображения приложения
    w.setWindowTitle("MathStat");
    w.showMaximized();

    //w.show();
    return a.exec();
}
