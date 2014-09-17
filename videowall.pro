SOURCES += main.cpp
SOURCES += VideoProxy.cpp WallWindow.cpp
HEADERS += VideoProxy.h WallWindow.h

RESOURCES += videowall.qrc
QT += gui phonon4qt5 opengl

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[QT_INSTALL_DEMOS]/videowall
sources.files = $$SOURCES $$HEADERS $$RESOURCES *.png *.pro *.html *.doc images
sources.path = $$[QT_INSTALL_DEMOS]/videowall
INSTALLS += target sources
