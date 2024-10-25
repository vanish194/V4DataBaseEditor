QT       += core sql gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    databasesaver.cpp \
    db_model/additionalmnemonic.cpp \
    db_model/company.cpp \
    db_model/conversionformula.cpp \
    db_model/mainmnemonic.cpp \
    db_model/method.cpp \
    db_model/producer.cpp \
    db_model/sensor.cpp \
    db_model/sensormnemonic.cpp \
    db_model/tool.cpp \
    db_model/tooldescription.cpp \
    db_model/toolmnemonic.cpp \
    db_model/toolsensor.cpp \
    db_model/unit.cpp \
    db_model/unittype.cpp \
    editor_windows/comparedatadialog.cpp \
    editor_windows/sensor_editor.cpp \
    editor_windows/sensoraddwindow.cpp \
    editor_windows/sensormnemonicrelationeditor.cpp \
    editor_windows/tool_editor.cpp \
    editor_windows/tooladdwindow.cpp \
    editor_windows/toolmnemonicrelationeditor.cpp \
    editor_windows/toolsensorrelationeditor.cpp \
    main.cpp \
    mainwindow.cpp \
    storage.cpp \
    storageeditor.cpp \
    views/toolsensormnemonictreeview.cpp

HEADERS += \
    databasemanager.h \
    databasesaver.h \
    db_model/additionalmnemonic.h \
    db_model/company.h \
    db_model/conversionformula.h \
    db_model/mainmnemonic.h \
    db_model/method.h \
    db_model/producer.h \
    db_model/sensor.h \
    db_model/sensormnemonic.h \
    db_model/tool.h \
    db_model/tooldescription.h \
    db_model/toolmnemonic.h \
    db_model/toolsensor.h \
    db_model/unit.h \
    db_model/unittype.h \
    editor_windows/comparedatadialog.h \
    editor_windows/sensor_editor.h \
    editor_windows/sensoraddwindow.h \
    editor_windows/sensormnemonicrelationeditor.h \
    editor_windows/tool_editor.h \
    editor_windows/tooladdwindow.h \
    editor_windows/toolmnemonicrelationeditor.h \
    editor_windows/toolsensorrelationeditor.h \
    mainwindow.h \
    storage.h \
    storageeditor.h \
    views/toolsensormnemonictreeview.h

FORMS += \
    editor_windows/comparedatadialog.ui \
    editor_windows/sensor_editor.ui \
    editor_windows/sensoraddwindow.ui \
    editor_windows/sensormnemonicrelationeditor.ui \
    editor_windows/tool_editor.ui \
    editor_windows/tooladdwindow.ui \
    editor_windows/toolmnemonicrelationeditor.ui \
    editor_windows/toolsensorrelationeditor.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
