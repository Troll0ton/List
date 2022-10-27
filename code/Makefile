all: mlist

mlist: main.o list.o 
	g++ main.o list.o 

main.o: main.cpp
	g++ -c main.cpp

list.o: list.cpp
	g++ -c list.cpp

clean:
	rm -rf *.o