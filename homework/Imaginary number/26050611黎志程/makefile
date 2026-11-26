.RECIPEPREFIX = >

CXX = g++
CXXFLAGS = -Wall -Wextra -g -O0

.PHONY: clean app

all: app

app: use_imagnum.o imagnum.o
>    $(CXX) use_imagnum.o imagnum.o -o app

use_imagnum.o: use_imagnum.cpp imagnum.h
>    $(CXX)$(CXXFLAGS) -c use_imagnum.cpp -o use_imagnum.o

imagnum.o: imagnum.cpp imagnum.h
>    $(CXX)$(CXXFLAGS) -c imagnum.cpp -o imagnum.o

clean:
>	rm -f *.o app
