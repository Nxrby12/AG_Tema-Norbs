cmake_minimum_required(VERSION 3.10)
project(DijkstraPathVisualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Xml REQUIRED)

add_executable(DijkstraPathVisualizer
    main.cpp
    MainWindow.cpp
    Graph.cpp
    XMLParser.cpp
    KDTree.cpp
)

target_link_libraries(DijkstraPathVisualizer
    Qt5::Core
    Qt5::Widgets
    Qt5::Xml
)
