CFLAGS = -Wall -fpic -coverage -lm -std=c99

runtest: testme.c
	gcc -o testme -g testme.c $(CFLAGS)
	./testme

clean:
	-rm -f *.o testme
