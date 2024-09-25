QT       += core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets printsupport

CONFIG += c++17 sdk_no_version_check

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Model/s21_smart_calculator.cc \
    main.cc \
    View/s21_calculator_view.cc \
    Controller/s21_calculator_controller.cc \
    qcustomplot.cpp

HEADERS += \
    Model/s21_smart_calculator.h \
    View/s21_calculator_view.h \
    Controller/s21_calculator_controller.h \
    qcustomplot.h

FORMS += \
    View/view.ui

ICON = "Ikon/calc.icns"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
