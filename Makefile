all: main.o MyTar.o
	g++ -o mytar main.o MyTar.o
main.o: main.cpp
	gcc -c main.cpp
MyTar.o: MyTar.cpp MyTar.h
	gcc -c MyTar.cpp
clean:
	rm -f *.o mytar
