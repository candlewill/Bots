echo "开始清理不必要提交到git的数据"

cd Bot_tools/Usage

cd build
rm -rf CMakeFiles
rm cmake_install.cmake
rm CMakeCache.txt

cd ../LOGS
rm *.txt

cd ../TMP
rm *.bin

cd ../TOPIC
rm -rf BUILD1

cd ../USERS
rm *.txt

cd ../VERIFY
rm *.txt