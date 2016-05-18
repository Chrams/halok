#Makefile for my networking project
CC=gcc
CFLAGS=-Wall -std=c99
SRC_DIR=src/

all: kviz

kviz:
	$(CC) $(CFLAGS) $(SRC_DIR)question.h $(SRC_DIR)player.h $(SRC_DIR)server.c -o kviz_server
	$(CC) $(CFLAGS) $(SRC_DIR)client.c -o kviz_client

clean:
	rm kviz_client kviz_server
