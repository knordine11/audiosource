TEMPLATE = app
TARGET = audiosource

QT += multimedia widgets
CONFIG += add_ios_ffmpeg_libraries

HEADERS       = audiosource.h \
    fftstuff.h \
    fftw3/fftw3.h \


SOURCES       = audiosource.cpp \
                fftstuff.cpp \
                main.cpp \


target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audiosource
INSTALLS += target
include(../shared/shared.pri)

DISTFILES += \
    fftw3/COPYRIGHT \
    fftw3/fftw3.f \
    fftw3/fftw3.f03 \
    fftw3/fftw3l.f03 \
    fftw3/fftw3q.f03 \
    fftw3/libfftw3-3.dll \
    fftw3/libfftw3f-3.dll \
    fftw3/libfftw3l-3.dll

INCLUDEPATH += $$PWD/fftw3
DEPENDPATH += $$PWD/fftw3

unix|win32: LIBS += -L$$PWD/fftw3/ -llibfftw3-3 -llibfftw3f-3 -llibfftw3l-3
