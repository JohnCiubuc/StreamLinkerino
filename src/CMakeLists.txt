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
    MainWindow.cpp
    WindowsMatchingPID.cpp
    Submodules.cpp
)
set(HEADERS
  MainWindow.h
  WindowsMatchingPID.h
    Submodules.h
)
set(UI 
MainWindow.ui
SubmodulesDialog.ui)




add_executable(streamlinkerino ${SOURCES} ${HEADERS} ${UI})
target_link_libraries(streamlinkerino Qt5::Widgets X11)
