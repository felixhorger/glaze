.PHONY: all

build/glad.o: src/glad.c include/glad.h include/khrplatform.h
	gcc -fPIC -c src/glad.c -o build/glad.o
#

build/glaze.o: src/glaze.c
	gcc -fPIC -c src/glaze.c -o build/glaze.o
#

lib/libglaze.so: build/glaze.o build/glad.o
	gcc -fPIC -shared -o $@ build/glaze.o build/glad.o -ldl -lGL
#

lib/libglaze.a: build/glaze.o build/glad.o
	ar rcs $@ build/glaze.o build/glad.o
#

all: lib/libglaze.so lib/libglaze.a

