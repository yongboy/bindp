
bindp.so: bindp.c
	gcc -nostartfiles -fpic -shared bindp.c -o bindp.so -ldl -D_GNU_SOURCE

clean:
	rm bindp.so -f
