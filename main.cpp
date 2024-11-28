#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QLocale>
#include <QSettings>
#include <QStyleFactory>
#include <QTranslator>
#include "mainwindow.h"

bool isDarkTheme()
{
    bool darkTheme = false;

#ifdef Q_OS_WIN
    // Открываем ключ реестра
    QSettings settings(
        "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        QSettings::NativeFormat);

    // Читаем значение параметра AppsUseLightTheme
    if (settings.contains("AppsUseLightTheme")) {
        int value = settings.value("AppsUseLightTheme").toInt();
        darkTheme = (value == 0); // 0 означает тёмную тему
    }
#endif

    return darkTheme;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Получаем текущую системную локаль
    QString locale = QLocale::system().name(); // Например, "ru_RU"

    QTranslator translator;
    QDir appDir(QCoreApplication::applicationDirPath());

    if (appDir.dirName() == "debug" || appDir.dirName() == "release") {
        appDir.cdUp();
    }

    // Загружаем перевод вашего приложения
    QString translationFile = appDir.filePath("translations/V4DataBaseEditor_" + locale + ".qm");

    if (translator.load(translationFile)) {
        qDebug() << "Файл перевода успешно загружен по пути:" << translationFile;
        app.installTranslator(&translator);
    } else {
        qWarning() << "Не удалось загрузить файл перевода по пути:" << translationFile;
    }

    // Загружаем стандартный перевод Qt
    QTranslator qtTranslator;
    if (qtTranslator.load("qtbase_" + locale, appDir.filePath("translations"))) {
        qDebug() << "Стандартный перевод Qt успешно загружен.";
        app.installTranslator(&qtTranslator);
    } else {
        qWarning() << "Не удалось загрузить стандартный перевод Qt.";
    }

    // Определяем текущую тему и применяем соответствующий стиль
    if (isDarkTheme()) {
        QFile styleFile(":/styles/dark.qss");
        if (styleFile.open(QFile::ReadOnly)) {
            QString style = QLatin1String(styleFile.readAll());
            app.setStyleSheet(style);
            qDebug() << "Применена тёмная тема.";
        } else {
            qWarning() << "Не удалось загрузить файл стилей для тёмной темы.";
        }
    } else {
        QFile styleFile(":/styles/light.qss");
        if (styleFile.open(QFile::ReadOnly)) {
            QString style = QLatin1String(styleFile.readAll());
            app.setStyleSheet(style);
            qDebug() << "Применена светлая тема.";
        } else {
            qWarning() << "Не удалось загрузить файл стилей для светлой темы.";
        }
    }

    MainWindow w;
    w.show();

    return app.exec();
}
