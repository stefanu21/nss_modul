VERSION = 0.0
CC = gcc
CFLAGS = -fPIC -shared -Wall -g3 -DVERSION=\"$(VERSION)\"
LIB = libnss_demo.so.2
LDFLAGS = -ljansson -Wl,-soname,$(LIB)
OBJ = main.o

$(LIB): $(OBJ)
	$(CC) $(CFLAGS) -o $(LIB) $(OBJ) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $<


.PHONY: all clean install
	
all: $(LIB)

clean:
	rm -rf $(LIB) $(OBJ)

install:
	sudo install -m 0644 $(LIB) /lib
	sudo /sbin/ldconfig -n /lib /usr/lib

