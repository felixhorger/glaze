.PHONY: all test

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

test/test: test/test.c lib/libglaze.a
	gcc -o test/test.o -c test/test.c
	gcc -o test/test test/test.o lib/libglaze.a -ldl -lm -lGL -lglfw
#

test: test/test



all: lib/libglaze.so lib/libglaze.a

