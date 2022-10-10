
prog: mainproc.o proc.o
	gcc mainproc.o proc.o -o prog

mainproc.o: mainproc.cpp
	gcc -c mainproc.cpp -o mainproc.o

proc.o: proc.cpp
	gcc -c proc.cpp -o proc.o

clean:
	rm prog *.o

