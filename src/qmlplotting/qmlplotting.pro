TEMPLATE = aux
CONFIG += c++14 force_qt thread warn_on
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QMLPLOTTING_LIBRARY
QT += qml quick

include($$PWD/qmlplotting.pri)
