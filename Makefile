build:
	g++ ./src/*.cpp ./src/*.h -o main -Wall -Wunused -lraylib -g
run:
	./main
clean: 
	rm ./main
