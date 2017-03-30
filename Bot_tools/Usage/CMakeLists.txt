# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.0)
# 项目信息
project (ChatScript_Main)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加头文件目录INCLUDE_DIRECTORIES
include_directories(./ChatScript/include)

# 添加math子目录
# 使用命令 add_subdirectory 指明本项目包含一个子目录 math，这样 math 目录下的 CMakeLists.txt 文件和源代码也会被处理
link_directories("./ChatScript")

# 指定生成目标
add_executable(ChatScript_Main Dialog.cpp)

# 添加链接库
# 使用命令 target_link_libraries 指明可执行文件 main 需要连接一个名为 MathFunctions 的链接库
target_link_libraries(ChatScript_Main ChatScript curl)