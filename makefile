CC = clang
CXX = clang++# Flag for implicit rules
CFLAGS = -g
CXXFLAGS = -I ./include

clean:
	rm -f ./build/*.o

edit:
	$(CXX) src/main.cpp -o build/main.o $(CPPFLAGS) $(CXXFLAGS)

run: edit
	./build/main.o