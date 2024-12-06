cmake_minimum_required(VERSION 2.8)	#声明cmake需要的最低版本
project(Demo)	#声明cmake项目的名称

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})	#添加OpenCV头文件
include_directories (include)
add_executable(${PROJECT_NAME} src/main.cpp src/getNumber.cpp src/machLight.cpp)	#生成可执行文件，这里我把可执行文件名称定义成项目名称
add_subdirectory (src)
aux_source_directory (src SRC_LIST)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})	#链接OpenCV库文件到cmake项目上
