CC := g++ -O2 -mavx2 -Wno-pointer-arith -Wwrite-strings
FAST := g++ -Ofast -mavx2 -Wno-pointer-arith -Wwrite-strings
SRCSSE := main.cpp SSE.cpp
SRCNOSSE := main.cpp No-SSE.cpp
SRCALPHA := alphamain.cpp alphablending.cpp
DIR := C:\Users\USER\Documents\Mandelbrot

.PHONY: all clean

main: $(SRC)
	$(CC) $^ -o $(DIR)\$@

NOSSE: $(SRCNOSSE)
	$(CC) $^ -o $(DIR)\$@

NOSSEf: $(SRCNOSSE)
	$(FAST) $^ -o $(DIR)\$@

SSE: $(SRCSSE)
	$(CC) $^ -o $(DIR)\$@

SSEf: $(SRCSSE)
	$(FAST) $^ -o $(DIR)\$@

alpha: $(SRCALPHA)
	$(CC) $^ -o $(DIR)\$@

alphaf: $(SRCALPHA)
	$(FAST) $^ -o $(DIR)\$@

clean:
	rm -rf *.png *.dot
