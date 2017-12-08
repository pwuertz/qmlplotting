TEMPLATE = subdirs
SUBDIRS = \
    src \
    tests

tests.depends = src

OTHER_FILES = \
    .qmake.conf
