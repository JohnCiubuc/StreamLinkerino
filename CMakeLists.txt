project(streamlinkerino) 

set(CMAKE_CXX_STANDARD 11) 
cmake_minimum_required(VERSION 3.18)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(X11 REQUIRED)

set(SOURCES
    main.cpp
    mainwindow.cpp
    windowsmatchingpid.cpp
)
set(HEADERS
    mainwindow.h
    windowsmatchingpid.h
)
set(UI mainwindow.ui)

add_executable(streamlinkerino ${SOURCES} ${HEADERS} ${UI})
target_link_libraries(streamlinkerino Qt5::Widgets X11)
