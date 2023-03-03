QT += core gui widgets openglwidgets

CONFIG += c++17

SOURCES += \
    face.cpp \
    glview.cpp \
    halfedge.cpp \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp \
    model.cpp \
    netcontroler.cpp \
    objreader.cpp \
    settingslider.cpp \
    vertex.cpp

HEADERS += \
    settingslider.h \
    shaders.h \
    face.h \
    glview.h \
    halfedge.h \
    mainwindow.h \
    mesh.h \
    model.h \
    netcontroler.h \
    objreader.h \
    vertex.h

#LIBS += -lopengl32 -lglu32

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
