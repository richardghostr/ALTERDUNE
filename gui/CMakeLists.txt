cmake_minimum_required(VERSION 3.16)
project(alterdune_qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.11 COMPONENTS Quick Qml QuickControls2 REQUIRED)

add_executable(alterdune_qt
    src/main_qt.cpp
    src/GameBridge.cpp
    qml.qrc
    
    # Core game sources (reuse existing project sources)
    ../src/Entity.cpp
    ../src/Player.cpp
    ../src/Monster.cpp
    ../src/Normal.cpp
    ../src/MiniBoss.cpp
    ../src/Boss.cpp
    ../src/Item.cpp
    ../src/Inventory.cpp
    ../src/ActAction.cpp
    ../src/Bestiary.cpp
    ../src/FileLoader.cpp
)

target_include_directories(alterdune_qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(alterdune_qt PRIVATE Qt6::Quick Qt6::Qml Qt6::QuickControls2)

# Install target (optional)
install(TARGETS alterdune_qt RUNTIME DESTINATION bin)
