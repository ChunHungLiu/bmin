# kernel include project file
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
SOURCES += formula.cpp \
    term.cpp \
    kernelexc.cpp \
    literalvalue.cpp \
    quinemccluskey.cpp \
    kernel.cpp \
    termscontainer.cpp \
    outputvalue.cpp \
    events.cpp \
    kmap.cpp \
    cube.cpp \
    espresso.cpp \
    espressocover.cpp
HEADERS += formula.h \
    term.h \
    kernelexc.h \
    literalvalue.h \
    minimizingalgorithm.h \
    quinemccluskey.h \
    kernel.h \
    constants.h \
    termscontainer.h \
    outputvalue.h \
    events.h \
    kmap.h \
    cube.h \
    espresso.h \
    espressocover.h
