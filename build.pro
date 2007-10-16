win32:TEMPLATE	= vclib
unix:TEMPLATE = lib
macx:TEMPLATE = lib
LANGUAGE	= C++

CONFIG	+= warn_on 

unix:CONFIG += staticlib
win32:CONFIG += staticlib

QT -= gui

include(src/__build__.pri)

TARGET = MorkParser


DESTDIR = dist
OBJECTS_DIR = obj

CONFIG(release) {
 DEFINES += QT_NO_DEBUG_OUTPUT
}


