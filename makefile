compiler := c++

lyre: lyre.o
	echo "Compiling binary..." 
	${compiler} lyre.o -o lyre -pthread

lyre.o: lyre.cc
	echo "Generating object file..."
	${compiler} -c lyre.cc -I"core/" -I"devices/"

clean:
	rm -f *.o
