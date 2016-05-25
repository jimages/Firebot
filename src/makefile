# test make file
# Author: Zachary Wang(jimages123@gmail.com)
CC = gcc
CFLAG = -std=c99 -ggdb -Wall
MYSQL_LINK = `mysql_config --libs`
MYSQL_COMP = `mysql_config --cflags`

all: test

test: config.o firebot.o log.o
	$(CC) $(CFLAG) -o firebot config.o firebot.o log.o $(MYSQL_LINK)

config.o: config.c include/firebot.h include/config.h
	$(CC) $(CFLAG) -c $(MYSQL_COMP) config.c 

firebot.o: firebot.c include/firebot.h include/config.h
	$(CC) $(CFLAG) -c $(MYSQL_COMP) firebot.c

log.o: log.c include/firebot.h include/log.h
	$(CC) $(CFLAG) -c $(MYSQL_COMP) log.c

clean:
	$(RM) *.o firebot 
