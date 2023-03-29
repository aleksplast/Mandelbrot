CC := g++ -Wno-pointer-arith -Wwrite-strings
SRC := main.cpp No-SSE.cpp
DIR := C:\Users\USER\Documents\Mandelbrot

.PHONY: all clean

main: $(SRC)
	$(CC) $^ -o $(DIR)\$@

clean:
	rm -rf *.png *.dot
