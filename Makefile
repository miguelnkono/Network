CC= gcc
CLIBS= -liphlpapi -lws2_32
CFLAGS= -g -Wall -Wextra -pedantic -O3 $(CLIBS)
OUTPUT_OPTION= -MMD -MP -o $@

SOUCRE= $(wildcard src/*.c linux/*.c)
OBJS= $(SOUCRE:.c=.o)
DEPS= $(SOURCE:.c=.d)
-include $(DEPS)

time_server: src/time_server.o
	$(CC) -o $@ $^

socket_api: src/socket_api.o
	$(CC) -o $@ $^

time: src/time.o
	$(CC) -o $@ $^

network_list: linux/unix_list.o
	$(CC) -o $@ $^

clean_network_list:
	rm -rf linux/*.o linux/*d network_list

all: $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -rf src/*.o src/*.d linux/*.o linux/*d socket_api  all  time time_server