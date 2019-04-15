SHELL=/bin/sh
# Where you want it installed when you do 'make install'
PREFIX=/usr/local/

CFLAGS+= -I/usr/local/include 
LDFLAGS +=

all: mnist

LIB_OBJS = mnist.o file.o

mnist: $(LIB_OBJS)
	${CC} ${CFLAGS} ${LDFLAGS} ${LIB_OBJS} -o mnist 

distclean: clean
clean:
	rm -f *.o mnist

