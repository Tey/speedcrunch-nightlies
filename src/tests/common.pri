DEFINES += SPEEDCRUNCH_VERSION=\\\"master\\\"

QT += widgets help
CONFIG += c++11

DEPENDPATH += . \
              .. \
              ../core \
              ../gui \
              ../locale \
              ../math \
              ../resources

INCLUDEPATH += . .. ../math ../core ../gui

HEADERS += ../core/book.h \
           ../core/constants.h \
           ../core/evaluator.h \
           ../core/functions.h \
           ../core/session.h \
           ../core/errors.h \
           ../core/manualserver.h \
           ../core/numberformatter.h \
           ../core/pageserver.h \
           ../core/settings.h \
           ../core/opcode.h \
           ../core/sessionhistory.h \
           ../core/variable.h \
           ../core/userfunction.h \
           ../math/floatcommon.h \
           ../math/floatconfig.h \
           ../math/floatconst.h \
           ../math/floatconvert.h \
           ../math/floaterf.h \
           ../math/floatexp.h \
           ../math/floatgamma.h \
           ../math/floathmath.h \
           ../math/floatincgamma.h \
           ../math/floatio.h \
           ../math/floatipower.h \
           ../math/floatlog.h \
           ../math/floatlogic.h \
           ../math/floatlong.h \
           ../math/floatnum.h \
           ../math/floatpower.h \
           ../math/floatseries.h \
           ../math/floattrig.h \
           ../math/hmath.h \
           ../math/number.h \
           ../math/quantity.cpp \
           ../math/rational.h \
           ../math/units.h \
           ../gui/manualwindow.h

SOURCES += ../core/book.cpp \
           ../core/constants.cpp \
           ../core/evaluator.cpp \
           ../core/functions.cpp \
           ../core/manualserver.cpp \
           ../core/numberformatter.cpp \
           ../core/pageserver.cpp \
           ../core/settings.cpp \
           ../core/session.cpp \
           ../core/sessionhistory.cpp \
           ../core/variable.cpp \
           ../core/userfunction.cpp \
           ../core/opcode.cpp \
           ../math/floatcommon.c \
           ../math/floatconst.c \
           ../math/floatconvert.c \
           ../math/floaterf.c \
           ../math/floatexp.c \
           ../math/floatgamma.c \
           ../math/floathmath.c \
           ../math/floatio.c \
           ../math/floatipower.c \
           ../math/floatlog.c \
           ../math/floatlogic.c \
           ../math/floatlong.c \
           ../math/floatnum.c \
           ../math/floatpower.c \
           ../math/floatseries.c \
           ../math/floattrig.c \
           ../math/floatincgamma.c \
           ../math/hmath.cpp \
           ../math/number.c \
           ../math/cmath.cpp \
           ../math/cnumberparser.cpp \
           ../math/quantity.cpp \
           ../math/rational.cpp \
           ../math/units.cpp \
           ../gui/manualwindow.cpp

