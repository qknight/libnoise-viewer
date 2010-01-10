cmake_minimum_required(VERSION 2.6)

# i would to see more projects using these defaults
ADD_DEFINITIONS( "-Wall -ansi" )
# ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
project(libnoise-view CXX C)

find_package(Qt4)

# see http://cmake.org/cmake/help/cmake2.6docs.html#module:FindQt4
# SET (QT_USE_QTOPENGL 1)
# SET (QT_USE_QTXML 1) 


SET( libnoise-view_SOURCES 
 src/mainWidget.cpp 
 src/noiseutils.cpp 
 src/main.cpp 
 src/renderThread.cpp 
 src/movableGraphicsScene.cpp 
 src/movableGraphicsView.cpp 
 src/renderJob.cpp 
 src/threadJobControl.cpp
)

SET( libnoise-view_MOC_HEADERS
  src/mainWidget.h
  src/renderThread.h
  src/threadJobControl.h
  src/movableGraphicsView.h
)

SET( libnoise-view_UIs
  src/ui/mainWidget.ui
)

INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/
  # QT4_WRAP_UI writes ui_whatever.h here (that is './' and uic is used)
  ${CMAKE_CURRENT_BINARY_DIR}
  # this is the external library used (includes of libnoise live here)
  "/usr/include/noise/"
)

# QT4_WRAP_UI(outfiles inputfile ... OPTIONS ...)
QT4_WRAP_UI(libnoise-view_SOURCES ${libnoise-view_UIs})

# macro QT4_WRAP_CPP(outfiles inputfile ... )
QT4_WRAP_CPP (libnoise-view_SOURCES ${libnoise-view_MOC_HEADERS}) 

ADD_EXECUTABLE(libnoise-view ${libnoise-view_SOURCES} )

# -lnoise if for the external library
TARGET_LINK_LIBRARIES( libnoise-view ${QT_LIBRARIES} "-lnoise" )

INSTALL ( TARGETS libnoise-view DESTINATION bin )
