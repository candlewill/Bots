#!/bin/sh

chmod +x ./ChatScript/BINARIES/LinuxChatScript64
cd ChatScript && BINARIES/LinuxChatScript64 livedata=../LIVEDATA system=LIVEDATA/SYSTEM users=../USERS logs=../LOGS port=1024
