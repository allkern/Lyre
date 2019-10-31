compiler := c++

lyre: lyre.o
	echo "Compiling binary..."
	${compiler} lyre.o -o lyre -pthread -lsfml-graphics -lsfml-window -lsfml-system

lyre.o: lyre.cc
	echo "Generating object file..."
	${compiler} -c lyre.cc -I"core/" -I"devices/" -I"/usr/include/"

clean:
	rm -f *.o
