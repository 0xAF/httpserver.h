.PHONY: test clean

all: httpserver.h

test: http-server
	./http-server & test/run > results.txt; kill %1; diff results.txt test/results.txt

http-server: test/main.c httpserver.h
	$(CC) -O3 test/main.c -o http-server

clean:
	@rm http-server
