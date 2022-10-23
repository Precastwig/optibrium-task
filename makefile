COMPILER=g++-9

all:
	$(COMPILER) -std=c++2a -o unit-tests -I ./include *.cpp ./src/*.cpp
	@echo "Linked all into executable"
