#!/usr/bin/env bash

BASEDIR=$(pwd)

printf "\n\n本脚本作用是：\n"
printf "\n\n【项目部署】把需要用到的头文件和静态库复制到对应文件夹中\n\n"

cd $BASEDIR


cp ./install/include/ChatScript/*.h ./deploy/include/
cp ./install/include/curl/*.h ./deploy/include/
cp ./install/include/DialogStatic/*.h ./deploy/include/

cp ./install/libs/ChatScript/*.a ./deploy/libs/
cp ./install/libs/curl/*.a ./deploy/libs/
cp ./install/libs/DialogStatic/*.a ./deploy/libs/