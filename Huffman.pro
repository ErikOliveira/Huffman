#-------------------------------------------------
#
# Project created by QtCreator 2015-10-07T01:46:40
#
#-------------------------------------------------

TEMPLATE = app

QT += qml quick

HEADERS += actionscenter.h \
    bitarray.h \
    huffnode.h \
    hufftree.h \
    huffman.h

SOURCES += main.cpp \
    actionscenter.cpp \
    bitarray.cpp \
    huffnode.cpp \
    hufftree.cpp \
    huffman.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
