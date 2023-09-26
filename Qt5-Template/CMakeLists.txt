cmake_minimum_required(VERSION 3.10)
project(QT_try VERSION 0.1 LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt5Dev/5.14.2/mingw73_64/lib")

#set(CMAKE_PREFIX_PATH "C:/Qt5Dev/5.14.2/msvc2017_64/lib")
#设置Windows程序启动默认不需要控制台CMD
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#设置UIC的查找路径，UIC默认会在引用ui头文件的源代码路径下查找ui文件，而我们的ui文件存放在/ui文件夹下
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)
#执行程序生成路径
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


#头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty)


# 查找Qt库 这里是其中的关键，如果没配置好bin/mscv的路径可能会找不到，也可能你安装Qt时没勾选安装
find_package(QT NAMES  Qt5 REQUIRED COMPONENTS Core Gui Widgets Network SerialPort PrintSupport Xml DataVisualization)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Network SerialPort PrintSupport Xml DataVisualization)

# 将各个文件夹下的所有文件打包
FILE(GLOB ${PROJECT_NAME}_HEADER "include/*.h")
FILE(GLOB ${PROJECT_NAME}_SOURCES "src/*.cpp")
FILE(GLOB ${PROJECT_NAME}_UI "ui/*.ui")
FILE(GLOB ${PROJECT_NAME}_QRC "qrc/*.qrc")




#add_executable(${PROJECT_NAME}$ src/main.cpp)

#（如果没有可以去掉"qrc/ND-GUI.rc"）
add_executable(${PROJECT_NAME}
        #"qrc/ND-GUI.rc"
        ${${PROJECT_NAME}_QRC}
        ${${PROJECT_NAME}_UI}
        ${${PROJECT_NAME}_HEADER}
        ${${PROJECT_NAME}_SOURCES})
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
            Qt5::Core
            Qt5::Gui
            Qt5::Widgets
            Qt5::Network
            Qt5::SerialPort
            Qt5::PrintSupport
            Qt5::Xml
            Qt5::DataVisualization
    )
else()
    #Linux的配置大多数需要链接pthread线程库
    target_link_libraries(${PROJECT_NAME}
            Qt5::Core
            Qt5::Gui
            Qt5::Widgets
            Qt5::Network
            Qt5::SerialPort
            Qt5::Xml
            Qt5::DataVisualization
            pthread
    )
endif()

#调用预编译器MOC，需要使用 QT5_WRAP_CPP宏
QT5_WRAP_CPP(${PROJECT_NAME}_SOURCE_MOC ${${PROJECT_NAME}_SOURCE})

#使用UIC处理.ui文件
QT5_WRAP_UI(${PROJECT_NAME}_UI_HEADER ${${PROJECT_NAME}_UI})

#使用RCC处理.qrc文件
QT5_ADD_RESOURCES(${PROJECT_NAME}_RCC ${${PROJECT_NAME}_UIC})

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()

    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
else()
    link_directories(/usr/local/lib)
endif ()