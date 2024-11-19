#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    // Путь к файлу перевода, корректируем имя файла и путь
    if (translator.load(":/translations/V4DataBaseEditor_ru.qm")) {
        qDebug() << "Файл перевода успешно загружен!";
        app.installTranslator(&translator);
    } else {
        qWarning() << "Не удалось загрузить файл перевода!";
        // по невыясненным обстоятельствам работает только на прямую
        if (translator.load("C:/progQT/V4DataBaseEditor/translations/V4DataBaseEditor_ru.qm")) {
            qDebug() << "Файл перевода успешно загружен напрямую.";
            app.installTranslator(&translator);
        } else {
            qWarning() << "Не удалось загрузить файл перевода напрямую!";
        }
    }
    MainWindow w;
    w.show();

    return app.exec();
}
