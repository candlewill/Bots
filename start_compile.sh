# -I ./SRC/
# g++ -lpthread -lrt -lcurl -funsigned-char SRC/*.cpp -O2 -oLinuxChatScript 2>err.txt
# g++ -funsigned-char SRC/*.cpp -O2 -DDISCARDDATABASE=1 -o LinuxChatScript -lpthread -lrt -lcurl 2>>err.txt


# Dependencies: 
# 1. libcurl (http://blog.csdn.net/qianghaohao/article/details/51684862)

cd ChatScript-7.3/SRC/

make clean
echo "Start Compiling"

make server
echo "Compile End"
