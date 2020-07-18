CC=g++
INCLUDE=-I../include/
CFLAGS=-c -g -Wall -std=c++11 $(INCLUDE)
CPPFLAGS=-E -D
LDFLAGS=-lssl -lcrypto -lmysqlclient -lpthread -lhiredis
TARGET=app
SRCFILE=$(wildcard *.cpp)
DESFILE=$(patsubst %.cpp,%.o,$(SRCFILE))
RM=-rm -rf


$(TARGET):$(DESFILE)
	$(CC) $^ $(LDFLAGS) -o $@

%.o:%.cpp
	$(CC) $(CFLAGS) $<

output:
	@echo "$(SRCFILE)"
	@echo "$(DESFILE)"

clean:
	$(RM) $(DESFILE)
	$(RM) $(TARGET)
