TARGET=libindp.so
CC=gcc
CFLAGS=-nostartfiles -fpic -shared -D_GNU_SOURCE -fstack-protector-all -Wall -Wextra -Wformat-security -O2 -D_FORTIFY_SOURCE=2 -fstack-clash-protection -fcf-protection
LDFLAGS=-ldl -Wl,-z,relro -Wl,-z,now -Wl,-z,defs -Wl,-z,noexecstack

all:
	$(CC) $(CFLAGS) bindp.c -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
