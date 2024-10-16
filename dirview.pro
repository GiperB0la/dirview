QT += widgets
requires(qtConfig(treeview))

SOURCES       = main.cpp \
    dirview.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/itemviews/dirview
INSTALLS += target

HEADERS += \
    dirview.h
