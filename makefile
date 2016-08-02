# test make file
# Author: Zachary Wang(jimages123@gmail.com)
CC = cc
CFLAG = -std=c99 -ggdb -Wall
MYSQL_LINK = `mysql_config --libs`
MYSQL_COMP = `mysql_config --cflags`
CURL_LINK = `curl-config --libs`
CURL_COMP = `curl-config --cflags`

all: build/firebot

build/firebot: build/config.o build/firebot.o build/log.o build/daemon.o
	$(CC) $(CFLAG) -o build/firebot build/config.o build/firebot.o build/log.o \
		build/daemon.o \
		$(MYSQL_LINK) $(CURL_LINK)

build/config.o: src/config.c src/include/firebot.h src/include/config.h
	$(CC) $(CFLAG) -c $(MYSQL_COMP) $(CURL_COMP) -o build/config.o \
		src/config.c

build/firebot.o: src/firebot.c src/include/firebot.h src/include/config.h
	$(CC) $(CFLAG) -c $(MYSQL_COMP) $(CURL_COMP) -o build/firebot.o \
		src/firebot.c

build/log.o: src/log.c src/include/firebot.h src/include/log.h
	$(CC) $(CFLAG) -c $(MYSQL_COMP) $(CURL_COMP) -o build/log.o \
		src/log.c

build/daemon.o: src/daemon.c src/include/firebot.h src/include/daemon.h
	$(CC) $(CFLAG) -c $(MYSQL_COMP) $(CURL_COMP) -o build/daemon.o src/daemon.c

clean:
	$(RM) build/firebot build/*.o
