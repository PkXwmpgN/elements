all: test

test: test_visitor.o
	g++ -o test test_visitor.o

test_visitor.o: test_visitor.cpp
	g++ -I../../ -std=c++14 -Wall -Wextra -Werror -c test_visitor.cpp

clean:
	 rm test_visitor.o test
