TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    detectAlg.cpp \ 
    globalInclude.cpp \
    model.cpp \
    feature.cpp \
    featureIO.cpp

HEADERS += \
    detectAlg.h \
    globalInclude.h \
    model.h \
    feature.h \
    featureIO.h


#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += opencv

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv
