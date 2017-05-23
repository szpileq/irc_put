.DEFAULT_GOAL = all

.PHONY: all client server
all: client server

server:
	$(MAKE) -C server

