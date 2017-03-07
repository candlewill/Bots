server: DEFINES+= -DLOCKUSERFILE=1  -DEVSERVER=1 -DEVSERVER_FORK=1  -DDISCARDPOSTGRES=1 -DDISCARDMONGO=1 -DDISCARDMYSQL=1 
server: PGLOAD= -pthread
server: INCLUDEDIRS=-Ievserver
server: all
server: EXECUTABLE=../BINARIES/ChatScript
server: CFLAGS=-c  -std=c++11 -Wall  -funsigned-char  -Wno-write-strings -Wno-char-subscripts -Wno-strict-aliasing 

debugserver: DEFINES+= -DLOCKUSERFILE=1 -DDISCARDPOSTGRES=1 -DDISCARDMONGO=1 -DDISCARDMYSQL=1  -DDEBUG -DEVSERVER=1 -DEVSERVER_FORK=1 
debugserver: PGLOAD= -pthread
debugserver: INCLUDEDIRS=-Ievserver
debugserver: all
debugserver: EXECUTABLE=../BINARIES/ChatScriptDebug
debugserver: CFLAGS=-c -std=c++11 -Wall  -funsigned-char  -Wno-write-strings -Wno-char-subscripts -Wno-strict-aliasing -g

standalone: DEFINES+=  -DDISCARDSERVER=1 -DDISCARDPOSTGRES=1 -DDISCARDMONGO=1 -DDISCARDMYSQL=1 -DEVSERVER=1 -DEVSERVER_FORK=1
standalone: PGLOAD= -pthread
standalone: INCLUDEDIRS=-Ievserver
standalone: all
standalone: EXECUTABLE=../BINARIES/ChatScript
standalone: CFLAGS=-c -std=c++11  -Wall  -funsigned-char  -Wno-write-strings -Wno-char-subscripts -Wno-strict-aliasing 

pgserver: DEFINES+= -DLOCKUSERFILE=1 -DEVSERVER=1 -DEVSERVER_FORK=1 -DDISCARDMONGO=1 -DDISCARDMYSQL=1
pgserver: PGLOAD= -lpq -pthread
pgserver: all
pgserver: EXECUTABLE=../BINARIES/ChatScriptpg
pgserver: INCLUDEDIRS=-Ievserver -Ipostgres
pgserver: CFLAGS=-c -std=c++11 -Wall  -funsigned-char  -Wno-write-strings -Wno-char-subscripts -Wno-strict-aliasing 

debugpgserver: DEFINES+= -DLOCKUSERFILE=1  -DDEBUG -DEVSERVER=1 -DEVSERVER_FORK=1  -DDISCARDMONGO=1  
debugpgserver: PGLOAD= -lpq -pthread
debugpgserver: INCLUDEDIRS=-Ievserver
debugpgserver: all
debugpgserver: EXECUTABLE=../BINARIES/ChatScriptpgDebug
debugpgserver: CFLAGS=-c -std=c++11 -Wall  -funsigned-char  -Wno-write-strings -Wno-char-subscripts -Wno-strict-aliasing -g

mongoserver: DEFINES+= -DLOCKUSERFILE=1 -DEVSERVER=1 -DEVSERVER_FORK=1 -DDISCARDPOSTGRES=1 -DDISCARDMYSQL=1
mongoserver: PGLOAD= -pthread
mongoserver: all
mongoserver: EXECUTABLE=../BINARIES/ChatScriptMongo
mongoserver: INCLUDEDIRS=-Iinclude -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0
mongoserver: CFLAGS=-c -std=c++11  -Wall  -funsigned-char  -Wno-write-strings -Wno-char-subscripts -Wno-strict-aliasing -g

debugmongoserver: DEFINES+= -DLOCKUSERFILE=1  -DDEBUG -DEVSERVER=1 -DEVSERVER_FORK=1  -DDISCARDPOSTGRES=1  -DDISCARDMYSQL=1
debugmongoserver: PGLOAD= -lpq -pthread
debugmongoserver: all
debugmongoserver: INCLUDEDIRS=-Iinclude -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0
debugmongoserver: EXECUTABLE=../BINARIES/ChatScriptMongoDebug
debugmongoserver: CFLAGS=-c -std=c++11 -Wall  -funsigned-char  -Wno-write-strings -Wno-char-subscripts -Wno-strict-aliasing -g

foreignserver: DEFINES+= -DLOCKUSERFILE=1  -DEVSERVER=1 -DEVSERVER_FORK=1  -DDISCARDPOSTGRES=1 -DDISCARDMONGO=1 -DDISCARDMYSQL=1 -DTREETAGGER=1
foreignserver: PGLOAD= -pthread -ltreetagger -L../treetagger
foreignserver: INCLUDEDIRS=-Ievserver
foreignserver: all
foreignserver: EXECUTABLE=../BINARIES/ChatScriptForeign
foreignserver: CFLAGS=-c -std=c++11 -Wall  -funsigned-char  -Wno-write-strings -Wno-char-subscripts -Wno-strict-aliasing 

mongotreetaggerserver: DEFINES+= -DLOCKUSERFILE=1  -DEVSERVER=1 -DEVSERVER_FORK=1  -DDISCARDPOSTGRES=1  -DDISCARDMYSQL=1 -DTREETAGGER=1
mongotreetaggerserver: PGLOAD= -pthread -ltreetagger -L../treetagger
mongotreetaggerserver: INCLUDEDIRS=-Ievserver -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0
mongotreetaggerserver: all
mongotreetaggerserver: EXECUTABLE=../BINARIES/ChatScriptMongoForeign
mongotreetaggerserver: CFLAGS=-c -std=c++11 -Wall  -funsigned-char  -Wno-write-strings -Wno-char-subscripts -Wno-strict-aliasing 

UNAME := $(shell uname)

CC=g++

ifeq ($(UNAME), Linux)
$(info ************ LINUX VERSION ************)
LDFLAGS= -L/usr/lib64 -lrt -lcurl  --verbose $(PGLOAD)
mongoserver: LDFLAGS+= -lcrypt -lsasl2 -L/usr/local/lib -lmongoc-1.0 -lbson-1.0 
endif

ifeq ($(UNAME), Darwin)
OSXRELEASE := $(shell defaults read loginwindow SystemVersionStampAsString)
$(info ************ MAC VERSION $OSXRELEASE detected ************)
LDFLAGS=-mmacosx-version-min=$(OSXRELEASE) -lcurl $(PGLOAD)  
CFLAGS=-mmacosx-version-min=$(OSXRELEASE) -c  -Wall  -funsigned-char -Wno-write-strings -Wno-char-subscripts 
endif

SOURCES=constructCode.cpp evserver.cpp csocket.cpp cs_ev.c dictionarySystem.cpp englishTagger.cpp factSystem.cpp json.cpp functionExecute.cpp english.cpp infer.cpp javascript.cpp jsmn.cpp markSystem.cpp mongodb.cpp os.cpp outputSystem.cpp patternSystem.cpp postgres.cpp privatesrc.cpp scriptCompile.cpp spellcheck.cpp secure.cpp systemVariables.cpp tagger.cpp testing.cpp textUtilities.cpp tokenSystem.cpp topicSystem.cpp userCache.cpp userSystem.cpp variableSystem.cpp mainSystem.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
 
all: $(SOURCES) binary

clean:
	-rm -f $(EXECUTABLE)
	-rm -f ../$(EXECUTABLE)
	-rm -f *.o

binary: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(DEFINES) $(INCLUDEDIRS) -o $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDEDIRS) $< -o $@
