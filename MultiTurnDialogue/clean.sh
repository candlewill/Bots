#!/usr/bin/env bash

BASEDIR=$(pwd)

printf "\n\n本程序的目的是删除编译和执行时产生的临时文件，以便repo上传\n\n"


printf "进入MultiTurnDialogue文件夹\n"
cd $BASEDIR

printf "删除./TMP文件夹\n"
rm ./TMP -rf

printf "清理./build文件夹\n"
rm ./build/CMakeFiles -rf
rm ./build/CMakeCache.txt

printf "清理./SourceCode/ChatScript/文件夹\n"
rm ./SourceCode/ChatScript/ChatScript-7.3/SRC/*.o

printf "清理./SourceCode/DialogStatic/文件夹\n"
rm ./SourceCode/DialogStatic/build/CMakeFiles -rf
rm ./SourceCode/DialogStatic/build/CMakeCache.txt

printf "清理编译临时文件完成\n\n\n"

printf "开始清理聊天时产生的日志文件\n\n"

printf "清理./ChatScriptData中的日志文件\n"
rm ./ChatScriptData/VERIFY/*.txt
rm ./ChatScriptData/USERS/*.txt
rm ./ChatScriptData/TMP/*.bin
rm ./ChatScriptData/LOGS/*.txt


printf "清理./SourceCode/DialogStatic/中的日志文件\n"
rm ./SourceCode/DialogStatic/TMP -rf

rm ./SourceCode/DialogStatic/ChatScriptData/VERIFY/*.txt
rm ./SourceCode/DialogStatic/ChatScriptData/USERS/*.txt
rm ./SourceCode/DialogStatic/ChatScriptData/TMP/*.bin
rm ./SourceCode/DialogStatic/ChatScriptData/LOGS/*.txt

printf "清理./SourceCode/ChatScript/ChatScript-7.3/中的日志文件\n"
rm ./SourceCode/ChatScript/ChatScript-7.3/LOGS/*.txt
rm ./SourceCode/ChatScript/ChatScript-7.3/TMP/*.bin
rm ./SourceCode/ChatScript/ChatScript-7.3/USERS/*.txt
rm ./SourceCode/ChatScript/ChatScript-7.3/VERIFY/*.txt

printf "\n===============清理完成=============\n"
