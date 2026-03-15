QT += core gui network svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
RC_FILE = resource.rc

include(../all_pathes.pri)
include($$BITCOIN_PRICE_FETCHER_PRI)

SOURCES += \
    main.cpp \
    bitcoin_biper_mainwindow.cpp \
    trading_table_model.cpp

HEADERS += \
    bitcoin_biper_mainwindow.h \
    trading_table_model.h

FORMS += \
    bitcoin_biper_mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../res/res.qrc
