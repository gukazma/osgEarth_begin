set(TARGET_NAME osgEarthQtApp)

# just for qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_MODULE_PATH "D:/ThirdParty/cmake" ${CMAKE_MODULE_PATH})

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        REQUIRED)
find_package(OpenSceneGraph 2.0.0 REQUIRED osgViewer osgDB osgUtil osgQt5 osgGA)
file(GLOB_RECURSE CPP_HEAD_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE CPP_SOUCE_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CPP_HEAD_FILES} ${CPP_SOUCE_FILES})

add_executable(${TARGET_NAME} ${CPP_HEAD_FILES} ${CPP_SOUCE_FILES})

set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME ${TARGET_NAME})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Apps)

target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

target_link_libraries(${TARGET_NAME} PRIVATE 
Qt5::Core
Qt5::Gui
Qt5::Widgets
Qt5::OpenGL
${OPENSCENEGRAPH_LIBRARIES}
)

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  ${OPENSCENEGRAPH_INCLUDE_DIRS}
)