TARGET=libindp.so

all:
	gcc -nostartfiles -fpic -shared bindp.c -o ${TARGET} -ldl -D_GNU_SOURCE

clean:
	rm ${TARGET} -f
