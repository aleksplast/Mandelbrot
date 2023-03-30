CC := g++ -O2 -mavx2 -Wno-pointer-arith -Wwrite-strings
SRCSSE := main.cpp SSE.cpp
SRCNOSSE := main.cpp No-SSE.cpp
DIR := C:\Users\USER\Documents\Mandelbrot

.PHONY: all clean

main: $(SRC)
	$(CC) $^ -o $(DIR)\$@

NOSSE: $(SRCNOSSE)
	$(CC) $^ -o $(DIR)\$@

SSE: $(SRCSSE)
	$(CC) $^ -o $(DIR)\$@

clean:
	rm -rf *.png *.dot
