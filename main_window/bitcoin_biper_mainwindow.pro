QT       += core gui network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


include(../all_pathes.pri)
include($$BITCOIN_PRICE_FETCHER_PRI)


SOURCES += \
    main.cpp \
    bitcoin_biper_mainwindow.cpp

HEADERS += \
    bitcoin_biper_mainwindow.h \

FORMS += \
    bitcoin_biper_mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
