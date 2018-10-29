QT += widgets network

FORMS += \
    fenetre.ui

HEADERS += \
    fenetre.h \
    socket.h \
    decompte.h \
    camera.h

SOURCES += \
    fenetre.cpp \
    main.cpp \
    socket.cpp \
    decompte.cpp \
    camera.cpp

target.path=/home/pi
INSTALLS += target
