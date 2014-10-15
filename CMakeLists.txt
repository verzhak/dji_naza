
cmake_minimum_required(VERSION 3.0)

project(dji_naza)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_AMV QUIET amv)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CXX_FLAGS "${CXX_FLAGS} ${PC_AMV_CFLAGS} -Wall -pipe -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}" CACHE string "" FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

qt5_wrap_ui(dialogs resources/main_window.ui resources/convert_dialog.ui resources/about_dialog.ui)
qt5_add_resources(resources resources/resources.qrc)

set(sources main.cpp ${dialogs} ${resources} ui/main_window.cpp ui/convert_dialog.cpp ui/about_dialog.cpp lib/base.cpp lib/raw_xml.cpp lib/super_stend_xml.cpp lib/kml.cpp ui/QProgressIndicator/QProgressIndicator.cpp)

add_executable(dji_naza ${sources})
qt5_use_modules(dji_naza Core Widgets)
target_link_libraries(dji_naza ${OpenCV_LIBS} ${PC_AMV_LIBRARIES} glapi)

