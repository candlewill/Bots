# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.0)

# 在CMakeLists.txt文件的开头部分增加几个SET语法行
# 让 CMake 支持 gdb 的设置也很容易，只需要指定 Debug 模式下开启 -g 选项：
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 项目信息
project (ChatScript_Main)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加头文件目录INCLUDE_DIRECTORIES
include_directories(./ChatScript/include)

# 添加math子目录
# 使用命令 add_subdirectory 指明本项目包含一个子目录 math，这样 math 目录下的 CMakeLists.txt 文件和源代码也会被处理
link_directories("./ChatScript")

# 指定生成目标
add_executable(ChatScript_Main Dialog.cpp Dialog.h test_Dialog.cpp)

SET (LIBHELLO_SRC Dialog.cpp Dialog.h)
ADD_LIBRARY (DialogStatic STATIC ${LIBHELLO_SRC})
ADD_LIBRARY (DialogShared SHARED ${LIBHELLO_SRC})

# 添加链接库
# 使用命令 target_link_libraries 指明可执行文件 main 需要连接一个名为 MathFunctions 的链接库
target_link_libraries(ChatScript_Main ChatScript curl)
target_link_libraries(DialogStatic ChatScript curl)
