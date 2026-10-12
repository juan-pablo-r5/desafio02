cmake_minimum_required(VERSION 3.19)
project(des02 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(des02
    main.cpp
    cadena.h
    cadena.cpp
    fecha.h
    fecha.cpp
)

target_link_libraries(des02
    PRIVATE
        Qt::Core
)

install(TARGETS des02
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
