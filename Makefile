CC := g++ -mavx2 -Wno-pointer-arith -Wwrite-strings
O3 := g++ -O3 -mavx2 -Wno-pointer-arith -Wwrite-strings
FAST := g++ -mavx2 -Ofast -Wno-pointer-arith -Wwrite-strings
SRCSSE := mandelbrot\main.cpp mandelbrot\SSE.cpp
SRCNOSSE := mandelbrot\main.cpp mandelbrot\No-SSE.cpp
SRCALPHA := alphablending\alphamain.cpp alphablending\alphablending.cpp
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

alpha3: $(SRCALPHA)
	$(O3) $^ -o $(DIR)\$@

alphaf: $(SRCALPHA)
	$(FAST) $^ -o $(DIR)\$@

alphaall: alpha alpha3 alphaf

clean:
	rm -rf *.png *.dot
