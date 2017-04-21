#!/usr/bin/env bash

BASEDIR=$(pwd)

printf "\n\n本程序的目的是：\n编译多轮对话相关代码，最终编译结果放在*./install*目录中\n\n\n"

cd $BASEDIR

printf "开始编译./SourceCode/ChatScript\n\n"

cd $BASEDIR/SourceCode/ChatScript
bash start_compile.sh
bash start_compile_lib.sh
cd $BASEDIR

printf "\n\n编译完成\n\n"

printf "\n\n复制头文件和静态库\n\n"
cp $BASEDIR/SourceCode/ChatScript/ChatScript-7.3/BINARIES/libChatScript.a $BASEDIR/SourceCode/DialogStatic/ChatScript/libChatScript.a
cp $BASEDIR/SourceCode/ChatScript/ChatScript-7.3/BINARIES/libChatScript.a $BASEDIR/install/libs/ChatScript/libChatScript.a

cp $BASEDIR/SourceCode/ChatScript/ChatScript-7.3/SRC/*.h $BASEDIR/install/include/ChatScript/
cp $BASEDIR/SourceCode/ChatScript/ChatScript-7.3/SRC/*.h $BASEDIR/SourceCode/DialogStatic/ChatScript/include/


printf "\n\n开始编译./SourceCode/DialogStatic/\n\n"

cd $BASEDIR/SourceCode/DialogStatic/build

make clean
/data/data_151/work_home/heyunchao/Libs/cmake/bin/cmake ..
make

cd $BASEDIR

printf "\n\n编译完成\n\n"

printf "\n\n复制头文件和静态库\n\n"

cp $BASEDIR/SourceCode/DialogStatic/build/lib/libDialogStatic.a $BASEDIR/install/libs/DialogStatic/libDialogStatic.a


cp $BASEDIR/SourceCode/DialogStatic/Dialog.h $BASEDIR/install/include/DialogStatic/Dialog.h

printf "\n\n编译完成\n\n"

printf "\n\n开始编译./\n\n"

cd build
make clean
/data/data_151/work_home/heyunchao/Libs/cmake/bin/cmake ..
make

printf "\n\n==============编译完成=================\n\n"