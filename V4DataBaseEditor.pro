QT       += core sql gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    db_model/additionalmnemonic.cpp \
    db_model/company.cpp \
    db_model/conversionformula.cpp \
    db_model/mainmnemonic.cpp \
    db_model/method.cpp \
    db_model/producer.cpp \
    db_model/sensor.cpp \
    db_model/tool.cpp \
    db_model/tooldescription.cpp \
    db_model/toolsensor.cpp \
    db_model/typeofunit.cpp \
    db_model/unit.cpp \
    main.cpp \
    mainwindow.cpp \
    storage.cpp \
    views/toolsensormnemonictreeview.cpp

HEADERS += \
    databasemanager.h \
    db_model/additionalmnemonic.h \
    db_model/company.h \
    db_model/conversionformula.h \
    db_model/mainmnemonic.h \
    db_model/method.h \
    db_model/producer.h \
    db_model/sensor.h \
    db_model/tool.h \
    db_model/tooldescription.h \
    db_model/toolsensor.h \
    db_model/typeofunit.h \
    db_model/unit.h \
    mainwindow.h \
    storage.h \
    views/toolsensormnemonictreeview.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
