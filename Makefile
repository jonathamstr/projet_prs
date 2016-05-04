
all: testClientU test

test: test.o foncUDP.o
	gcc test.o foncUDP.o -o test

test.o: test.c
	gcc -c test.c -o test.o
	
testClientU.o: testClientU.c
	gcc -c testClientU.c -o testClientU.o

foncUDP.o: foncUDP.c foncUDP.h 
	gcc -c foncUDP.c -o foncUDP.o

testClientU: testClientU.o foncUDP.o
	gcc  testClientU.o foncUDP.o -o testClientU

